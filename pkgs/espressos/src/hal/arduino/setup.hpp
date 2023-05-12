#pragma once

#include <Adafruit_MAX31865.h>
#include <Arduino.h>
#include <LittleFS.h>
#include <PIDController.hpp>
#include <SimpleKalmanFilter.h>
#include <WiFi.h>
#include <cstdint>
#include <dimmable_light.h>

#include "../../api/handler.hpp"
#include "../../fsm//fsmlist.hpp"
#include "../../lib/effects.hpp"
#include "../../lib/map.hpp"
#include "../../lib/timers.hpp"
#include "ota.hpp"
#include "webserver.hpp"

void setupArduinoPressureSensor(Timers &timers) {
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
  // This sets the tolerance to the floor/ceiling +- 30%, it Works For Me™,
  // YMMV.
  static std::uint16_t minFloor = floor * 0.7;
  static std::uint16_t maxCeil = ceil * 1.3;

  timers.createInterval(PRESSURE_SENSOR_INTERVAL, []() {
    int value = analogRead(PRESSURE_SENSOR_PIN);

    // Clamp values outside of range to their respective max values
    if (value < floor && value >= minFloor) {
      value = floor;
    } else if (value > ceil && value <= maxCeil) {
      value = ceil;
    }

    if (value < floor || value > ceil) {
      send_event(PanicEvent());
      return;
    }

    value = pressureKalmanFilter.updateEstimate(value);

    PressureEvent e;
    e.pressure = (value - floor) * PRESSURE_SENSOR_MBAR / ceil;
    send_event(e);
  });
}

void setupArduinoTempSensor(Timers &timers) {
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

void setupArduinoSolenoid(Effects &effects) {
  pinMode(BREW_SOLENOID_PIN, OUTPUT);
  effects.createEffect<bool>(
      []() { return MachineState::current_state_ptr->getSolenoid(); },
      [](bool pinStatus) { digitalWrite(BREW_SOLENOID_PIN, pinStatus); });
}

void setupArduinoPump(Effects &effects) {
  static DimmableLight pump(PUMP_DIMMER_OUT);

  DimmableLight::setSyncPin(PUMP_DIMMER_ZC);
  DimmableLight::begin();

  static PumpTarget lastPumpTarget = (PumpTarget){PumpMode::POWER, 0};
  static PIDController<int32_t, float, unsigned long> pressureProfilePID(
      0, 1, 7, 0, DIRECT);

  pressureProfilePID.Begin(AUTOMATIC, millis());

  static uint8_t pumpPower = 0;

  // React to state machine pump changes
  effects.createEffect<PumpTarget>(
      []() { return MachineState::current_state_ptr->getPump(); },
      [](PumpTarget pumpTarget) {
        switch (pumpTarget.mode) {
        case PumpMode::POWER:
          // Remap value into uint8 range used by dimmer and apply
          pumpPower = map<uint16_t>(pumpTarget.value, 0, 65535, 0, 255);
          break;

        case PumpMode::PRESSURE:
          pressureProfilePID.SetSetpoint(pumpTarget.value);
          break;

        default:
          // We don't know what we're doing, the only safe thing to do is to
          // stop.
          send_event(PanicEvent());
        }

        lastPumpTarget = pumpTarget;
      });

  // Run pump pressure profiling PID loop
  effects.createEffect<unsigned long>(
      []() { return MachineState::current_state_ptr->getTimestamp(); },
      [](unsigned long timestamp) {
        if (lastPumpTarget.mode != PumpMode::PRESSURE) {
          return;
        }

        int32_t output;
        if (pressureProfilePID.Compute(
                timestamp, MachineState::current_state_ptr->getPressure(),
                &output)) {
          pumpPower = output;
        }
      });

  // Write pump power
  effects.createEffect<uint8_t>(
      []() { return pumpPower; },
      [](uint8_t power) { pump.setBrightness(power); });
}

void setupArduinoBoiler(Effects &effects, Timers &timers) {
  static constexpr int WindowSize = 100;
  static PIDController<int32_t, float, unsigned long> boilerPID(
      0, BOILER_PID_P, BOILER_PID_I, BOILER_PID_D, REVERSE);
  boilerPID.Begin(AUTOMATIC, millis());
  static unsigned long windowStartTime = millis();

  effects.createEffect<std::uint16_t>(
      []() { return MachineState::current_state_ptr->getSetPoint(); },
      [](std::uint16_t setpoint) { boilerPID.SetSetpoint(setpoint); });

  pinMode(BOILER_SSR_PIN, OUTPUT);

  static bool outputState = false;

  timers.createInterval(1, []() {
    unsigned long now = MachineState::current_state_ptr->getTimestamp();
    std::int16_t temp = MachineState::current_state_ptr->getTemp();
    std::int32_t Output;

    boilerPID.Compute(now, temp, &Output);

    if (now - windowStartTime >= WindowSize) {
      windowStartTime += WindowSize;
    }

    if (Output < now - windowStartTime) {
      if (!outputState) {
        digitalWrite(BOILER_SSR_PIN, HIGH);
      }
      outputState = true;
    } else {
      if (outputState) {
        digitalWrite(BOILER_SSR_PIN, LOW);
      }
      outputState = false;
    }
  });
};

void setupArduinoHAL(Timers &timers, Effects &effects) {
  // Turn on board power LED
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  setupArduinoPressureSensor(timers);
  setupArduinoTempSensor(timers);
  setupArduinoSolenoid(effects);
  setupArduinoPump(effects);
  setupArduinoBoiler(effects, timers);
}

void setupArduinoAPI(APIHandler &handler, Timers &timers, Effects &effects,
                     PersistedConfig &pConfig,
                     StateUpdateMessage_t &stateUpdateMessage) {
  LittleFS.begin();

  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID_NAME, SSID_PASWORD);

  beginArduinoOTA(2040);

  static APIWebServer apiServer = APIWebServer(HTTP_PORT, &handler);
  apiServer.begin();

  // Set state update interval
  {
    static auto stateUpdateTimer =
        timers.createInterval(STATE_UPDATE_INTERVAL, [&stateUpdateMessage]() {
          apiServer.broadcastState(stateUpdateMessage);
        });

    effects.createEffect<unsigned long>(
        []() {
          return MachineState::current_state_ptr->getStateUpdateInterval();
        },
        [](unsigned long updateInterval) {
          stateUpdateTimer->interval = updateInterval;
        },
        false); // The update interval is irrelevant to the API server

    // If any watched effects triggered send update to clients
    effects.onTriggered([]() { stateUpdateTimer->last = 0; });
  }

  // Broadcast config on change
  pConfig.onChange([](Config config) { apiServer.broadcastConfig(config); });

  // Broadcaste config/state on new connections
  apiServer.onConnect([&pConfig, &stateUpdateMessage]() {
    apiServer.broadcastConfig(pConfig.getConfig());
    apiServer.broadcastState(stateUpdateMessage);
  });

  // Run loop functions on every loop() call
  timers.createInterval(0, []() {
    handleArduinoOTA();
    apiServer.loop();
  });
}
