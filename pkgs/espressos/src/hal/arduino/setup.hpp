#pragma once

#include <Adafruit_MAX31865.h>
#include <Arduino.h>
#include <LittleFS.h>
#include <ReadBufferFixedSize.h>
#include <SimpleKalmanFilter.h>
#include <WiFi.h>
#include <WriteBufferFixedSize.h>
#include <cstdint>
#include <dimmable_light.h>

#include <EventLoop.hpp>
#include <Map.hpp>
#include <Timers.hpp>
#include <fsmlist.hpp>
#include <signals.hpp>

#include "../../api/handler.hpp"
#include "../../reconcile/boiler.hpp"
#include "../../reconcile/pump.hpp"
#include "ota.hpp"
#include "webserver.hpp"

#define CONFIG_FILE "config.pb"
#define CONFIG_BUF_SIZE 128

constexpr int OTAPort = 2040;
constexpr int SerialBaudRate = 115200;
constexpr std::uint16_t ADCMax = 1024;

constexpr float PressureFilterMeaE = 0.6F;
constexpr float PressureFilterEstE = 0.6F;
constexpr float PressureFilterQ = 0.1F;

static APIWebServer apiServer = APIWebServer(HTTP_PORT);

void setupArduinoPressureSensor() {
  // TODO: Don't hardcode filter values and consider where this filter actually
  // belongs
  static SimpleKalmanFilter pressureKalmanFilter(
      PressureFilterMeaE, PressureFilterEstE, PressureFilterQ);

  // Calculate the usable range of values from the ADC
  // A value of 0.8 means that we "lose" 20% of ADC range
  // as the sensor has a voltage range from 0.5v to 4.5v with a VCC of 5v.
  static const std::uint16_t floor = static_cast<std::uint16_t>(
      (ADCMax - (ADCMax * PRESSURE_SENSOR_RANGE)) / 2);
  static const std::uint16_t ceil = ADCMax - floor;

  // We can calculate the floor nicely, but in reality these values have some
  // tolerances so it's best to allow for some reads outside of the measured
  // range.
  //
  // This sets the tolerances so that we can detect floating voltage.
  static const std::uint16_t minFloor = 10;
  static const std::uint16_t maxCeil = ADCMax - minFloor;

  getEventLoop().createInterval(PRESSURE_SENSOR_INTERVAL, []() {
    int value = analogRead(PRESSURE_SENSOR_PIN);

    // Clamp values outside of range to their respective max values
    bool clamped = false;
    if (value < floor && value >= minFloor) { // NOLINT(bugprone-branch-clone)
      value = floor;
      clamped = true;
    } else if (value > ceil && value <= maxCeil) {
      value = ceil;
      clamped = true;
    }

    if (value < floor || value > ceil) {
      PanicEvent event;

      if (value < floor) {
        event.reason = "Pressure sensor underflow";
      } else {
        event.reason = "Pressure sensor overflow";
      }

      send_event(event);

      return;
    }

    if (!clamped) {
      value = static_cast<int>(
          pressureKalmanFilter.updateEstimate(static_cast<float>(value)));
    }

    auto pressureMbar = (value - floor) * PRESSURE_SENSOR_MBAR / ceil;
    send_event(PressureEvent(pressureMbar));
  });
}

void setupArduinoTempSensor() {
  static Adafruit_MAX31865 thermo(BOILER_MAX31865_SPI_PIN, BOILER_SPI_CLASS);
  thermo.begin();

  // Read temp and issue events
  getEventLoop().createInterval(PRESSURE_SENSOR_INTERVAL, []() {
    uint16_t rtd = 0;
    if (!thermo.readRTDAsync(rtd)) {
      return;
    };

    auto fault = thermo.readFault();

    // NOLINTBEGIN(readability-implicit-bool-conversion)
    if (fault) {
      thermo.clearFault();

      std::string reason = "RTD Error: ";
      if (fault & MAX31865_FAULT_HIGHTHRESH) {
        reason += "RTD High Threshold\n";
      }
      if (fault & MAX31865_FAULT_LOWTHRESH) {
        reason += "RTD Low Threshold\n";
      }
      if (fault & MAX31865_FAULT_REFINLOW) {
        reason += "REFIN- > 0.85 x Bias";
      }
      if (fault & MAX31865_FAULT_REFINHIGH) {
        reason += "REFIN- < 0.85 x Bias - FORCE- open\n";
      }
      if (fault & MAX31865_FAULT_RTDINLOW) {
        reason += "RTDIN- < 0.85 x Bias - FORCE- open\n";
      }
      if (fault & MAX31865_FAULT_OVUV) {
        reason += "Under/Over voltage\n";
      }

      send_event(PanicEvent(reason));
      return;
    }

    // Normalised temp (130 degrees celsius -> 13000)
    auto tempCelsius = static_cast<int16_t>(
        thermo.temperatureAsync(rtd, BOILER_RNOMINAL, BOILER_RREF) *
        100); // NOLINT(readability-magic-numbers)
              // NOLINT(cppcoreguidelines-avoid-magic-numbers)
    send_event(TempEvent(tempCelsius));
    // NOLINTEND(readability-implicit-bool-conversion)
  });
}

void setupArduinoSolenoid() {
  pinMode(BREW_SOLENOID_PIN, OUTPUT);
  MachineSignals::solenoid.createEffect([](const bool &pinStatus) {
    digitalWrite(BREW_SOLENOID_PIN, static_cast<int>(pinStatus));
  });
}

void setupArduinoPump() {
  static DimmableLight pump(PUMP_DIMMER_OUT);

  DimmableLight::setSyncPin(PUMP_DIMMER_ZC);
  DimmableLight::begin();

  static Signal<uint8_t> pumpPower(0);
  pumpPower.createEffect(
      [](const uint8_t &power) { pump.setBrightness(power); });

  setupPumpPID(pumpPower);
}

void setupArduinoBoiler() {
  static Signal<bool> outputState(false);

  setupBoilerPID(outputState);

  pinMode(BOILER_SSR_PIN, OUTPUT);
  outputState.createEffect([](const bool &status) {
    digitalWrite(BOILER_SSR_PIN, static_cast<int>(status));
  });
};

void setupArduinoConfig() {
  // Note the ordering of the code blocks here, we don't want to set up the
  // effect before we read the config from flash as that would be an infinite
  // loop.

  // Attempt to read config from flash
  auto file = LittleFS.open(CONFIG_FILE, "r");
  if (file) {
    Config config;

    EmbeddedProto::ReadBufferFixedSize<CONFIG_BUF_SIZE> buf;
    file.read(buf.get_data(), CONFIG_BUF_SIZE);
    file.close();
    buf.set_bytes_written(file.size());

    auto status = config.deserialize(buf);
    if (status != ::EmbeddedProto::Error::NO_ERRORS) {
      Serial.printf("Error decoding config: %d\n", status);
      return;
    }

    send_event(ConfigSetEvent(config));
  }

  static EmbeddedProto::WriteBufferFixedSize<CONFIG_BUF_SIZE> buf;

  // Set up update handler to save config
  ::MachineSignals::config.createEffect(
      [](const Config &config) {
        auto file = LittleFS.open(CONFIG_FILE, "w");
        if (!file) {
          logger->log(LogLevel::ERROR,
                      "Could not open config file for writing");
          return;
        }

        auto bytesWritten = file.write(buf.get_data(), buf.get_size());
        file.close();

        if (bytesWritten != buf.get_size()) {
          logger->log(LogLevel::ERROR,
                      "Error writing config file (n != buf.size))");
          return;
        }
      },
      false);
};

void setupArduinoHAL() {
  Serial.begin(SerialBaudRate);

  // Turn on board power LED
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  setupArduinoPressureSensor();
  setupArduinoTempSensor();
  setupArduinoSolenoid();
  setupArduinoPump();
  setupArduinoBoiler();
  setupArduinoConfig();
}

void setupArduinoAPI() {
  LittleFS.begin();

  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID_NAME, SSID_PASWORD);

  beginArduinoOTA(OTAPort);

  static StateUpdateMessage_t stateUpdateMessage;

  apiServer.begin();

  // Send state updates periodically
  {
    static auto stateUpdateTimer =
        getEventLoop().createInterval(STATE_UPDATE_INTERVAL, []() {
          updateStateUpdateMessage(stateUpdateMessage);
          apiServer.broadcastState(stateUpdateMessage);
        });

    // Set state update interval depending in the machine mode
    ::MachineSignals::mode.createEffect([](const MachineMode &mode) {
      if (mode == MachineMode::UNKNOWN || mode == MachineMode::PANIC ||
          mode == MachineMode::OFF || mode == MachineMode::IDLE) {
        stateUpdateTimer->interval = STATE_UPDATE_INTERVAL;
      } else {
        stateUpdateTimer->interval = STATE_UPDATE_INTERVAL_ACTIVE;
      }
    });

    // TODO: Reimplement state upate trigger on interesting event updates
    // If any watched effects triggered send update to clients
    // effects.onTriggered([]() { stateUpdateTimer->last = 0; });
  }

  // Broadcast config on change
  ::MachineSignals::config.createEffect(
      [](const Config &config) { apiServer.broadcastConfig(config); });

  // Broadcaste config/state on new connections
  apiServer.onConnect([]() {
    apiServer.broadcastConfig(::MachineSignals::config.get());
    updateStateUpdateMessage(stateUpdateMessage);
    apiServer.broadcastState(stateUpdateMessage);
  });
}

// On arduino the main loop is called implicitly through loop()
void loop() {
  getEventLoop().loop(millis());
  handleArduinoOTA();
  apiServer.loop();
}
