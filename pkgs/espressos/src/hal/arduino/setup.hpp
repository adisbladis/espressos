#pragma once

#include <Adafruit_MAX31865.h>
#include <Arduino.h>
#include <LittleFS.h>
#include <PIDController.hpp>
#include <ReadBufferFixedSize.h>
#include <SimpleKalmanFilter.h>
#include <WiFi.h>
#include <WriteBufferFixedSize.h>
#include <cstdint>
#include <dimmable_light.h>

#include "../../api/handler.hpp"
#include "../../fsm//fsmlist.hpp"
#include "../../fsm//signals.hpp"
#include "../../lib/map.hpp"
#include "../../lib/timers.hpp"
#include "../../reconcile/boiler.hpp"
#include "../../reconcile/pump.hpp"
#include "ota.hpp"
#include "webserver.hpp"

#define CONFIG_FILE "config.pb"
#define CONFIG_BUF_SIZE 128

static APIWebServer apiServer = APIWebServer(HTTP_PORT);

void setupArduinoPressureSensor() {
  // TODO: Don't hardcode filter values and consider where this filter actually
  // belongs
  static SimpleKalmanFilter pressureKalmanFilter(0.6f, 0.6f, 0.1f);

  // Calculate the usable range of values from the ADC
  // A value of 0.8 means that we "lose" 20% of ADC range
  // as the sensor has a voltage range from 0.5v to 4.5v with a VCC of 5v.
  static std::uint16_t floor = (1024 - (1024 * PRESSURE_SENSOR_RANGE)) / 2;
  static std::uint16_t ceil = 1024 - floor;

  // We can calculate the floor nicely, but in reality these values have some
  // tolerances so it's best to allow for some reads outside of the measured
  // range.
  //
  // This sets the tolerances so that we can detect floating voltage.
  static std::uint16_t minFloor = 10;
  static std::uint16_t maxCeil = 1024 - minFloor;

  timers.createInterval(PRESSURE_SENSOR_INTERVAL, []() {
    int value = analogRead(PRESSURE_SENSOR_PIN);

    // Clamp values outside of range to their respective max values
    bool clamped = false;
    if (value < floor && value >= minFloor) {
      value = floor;
      clamped = true;
    } else if (value > ceil && value <= maxCeil) {
      value = ceil;
      clamped = true;
    }

    if (value < floor || value > ceil) {
      send_event(PanicEvent());
      return;
    }

    if (!clamped) {
      value = pressureKalmanFilter.updateEstimate(value);
    }

    PressureEvent e;
    e.pressure = (value - floor) * PRESSURE_SENSOR_MBAR / ceil;
    send_event(e);
  });
}

void setupArduinoTempSensor() {
  static Adafruit_MAX31865 thermo(BOILER_MAX31865_SPI_PIN, BOILER_SPI_CLASS);
  thermo.begin();

  // Read temp and issue events
  timers.createInterval(PRESSURE_SENSOR_INTERVAL, []() {
    uint16_t rtd;
    bool cond = thermo.readRTDAsync(rtd);

    if (cond) {
      auto fault = thermo.readFault();

      if (fault) {
        thermo.clearFault();
        send_event(PanicEvent());
        return;
      }

      TempEvent tempEvent;
      tempEvent.temp =
          thermo.temperatureAsync(rtd, BOILER_RNOMINAL, BOILER_RREF) * 100;
      send_event(tempEvent);
    }
  });
}

void setupArduinoSolenoid() {
  pinMode(BREW_SOLENOID_PIN, OUTPUT);
  MachineSignals::solenoid.createEffect(
      [](bool pinStatus) { digitalWrite(BREW_SOLENOID_PIN, pinStatus); });
}

void setupArduinoPump() {
  static DimmableLight pump(PUMP_DIMMER_OUT);

  DimmableLight::setSyncPin(PUMP_DIMMER_ZC);
  DimmableLight::begin();

  static Signal<uint8_t> pumpPower(0);
  pumpPower.createEffect([](uint8_t power) { pump.setBrightness(power); });

  setupPumpPID(pumpPower);
}

void setupArduinoBoiler() {
  static Signal<bool> outputState(false);

  setupBoilerPID(outputState);

  pinMode(BOILER_SSR_PIN, OUTPUT);
  outputState.createEffect(
      [](bool status) { digitalWrite(BOILER_SSR_PIN, status); });
};

void setupArduinoConfig() {
  // Note the ordering of the code blocks here, we don't want to set up the
  // effect before we read the config from flash as that would be an infinite
  // loop.

  // Attempt to read config from flash
  File f = LittleFS.open(CONFIG_FILE, "r");
  if (f) {
    Config config;

    EmbeddedProto::ReadBufferFixedSize<CONFIG_BUF_SIZE> buf;
    f.read(buf.get_data(), CONFIG_BUF_SIZE);
    buf.set_bytes_written(f.size());
    auto status = config.deserialize(buf);
    if (status != ::EmbeddedProto::Error::NO_ERRORS) {
      Serial.printf("Error decoding config: %d\n", status);
      return;
    }

    ConfigSetEvent configSetEvent;
    configSetEvent.config = config;
    send_event(configSetEvent);
  }

  static EmbeddedProto::WriteBufferFixedSize<CONFIG_BUF_SIZE> buf;

  // Set up update handler to save config
  ::MachineSignals::config.createEffect(
      [](const Config &config) {
        File f = LittleFS.open(CONFIG_FILE, "w");
        if (!f) {
          logger->log(LogLevel::ERROR,
                      "Could not open config file for writing");
          return;
        }

        auto n = f.write(buf.get_data(), buf.get_size());
        f.close();

        if (n != buf.get_size()) {
          logger->log(LogLevel::ERROR,
                      "Error writing config file (n != buf.size))");
          return;
        }
      },
      false);
};

void setupArduinoHAL() {
  Serial.begin(115200);

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

  beginArduinoOTA(2040);

  static StateUpdateMessage_t stateUpdateMessage;
  setupAPIEffects(stateUpdateMessage);

  apiServer.begin();

  // Send state updates periodically
  {
    static auto stateUpdateTimer =
        timers.createInterval(STATE_UPDATE_INTERVAL, []() {
          apiServer.broadcastState(stateUpdateMessage);
        });

    // Set state update interval depending in the machine mode
    ::MachineSignals::mode.createEffect([](MachineMode mode) {
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
    apiServer.broadcastState(stateUpdateMessage);
  });
}

// On arduino the main loop is called implicitly through loop()
void loop() {
  timers.loop(millis());
  handleArduinoOTA();
  apiServer.loop();
}
