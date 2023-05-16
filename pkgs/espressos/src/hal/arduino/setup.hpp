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
#include "../../fsm//signals.hpp"
#include "../../lib/map.hpp"
#include "../../lib/timers.hpp"
#include "../../reconcile/boiler.hpp"
#include "../../reconcile/pump.hpp"
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

void setupArduinoSolenoid() {
  pinMode(BREW_SOLENOID_PIN, OUTPUT);
  MachineSignals::solenoid.createEffect(
      [](bool pinStatus) { digitalWrite(BREW_SOLENOID_PIN, pinStatus); });
}

void setupArduinoPump(Timers &timers) {
  static DimmableLight pump(PUMP_DIMMER_OUT);

  DimmableLight::setSyncPin(PUMP_DIMMER_ZC);
  DimmableLight::begin();

  static Signal<uint8_t> pumpPower(0);
  pumpPower.createEffect([](uint8_t power) { pump.setBrightness(power); });

  setupPumpPID(timers, pumpPower);
}

void setupArduinoBoiler(Timers &timers) {
  static Signal<bool> outputState(false);

  setupBoilerPID(timers, outputState);

  pinMode(BOILER_SSR_PIN, OUTPUT);
  outputState.createEffect(
      [](bool status) { digitalWrite(BOILER_SSR_PIN, status); });
};

void setupArduinoHAL(Timers &timers) {
  // Turn on board power LED
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  setupArduinoPressureSensor(timers);
  setupArduinoTempSensor(timers);
  setupArduinoSolenoid();
  setupArduinoPump(timers);
  setupArduinoBoiler(timers);
}

void setupArduinoAPI(APIHandler &handler, Timers &timers,
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

    ::MachineSignals::stateUpdateInterval.createEffect(
        [](Timestamp_t updateInterval) {
          stateUpdateTimer->interval = updateInterval;
        });

    // TODO: Reimplement state upate trigger on interesting event updates
    // If any watched effects triggered send update to clients
    // effects.onTriggered([]() { stateUpdateTimer->last = 0; });
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
