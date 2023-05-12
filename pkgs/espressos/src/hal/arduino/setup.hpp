#pragma once

#include "../../fsm//fsmlist.hpp"
#include <Arduino.h>
#include <Adafruit_MAX31865.h>
#include <SimpleKalmanFilter.h>
#include <cstdint>

#include "../../lib/timers.hpp"

void setupArduinoPressureSensor(Timers &timers) {
  // TODO: Don't hardcode filter values and consider where this filter actually belongs
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

void setupHAL(Timers &timers) {
  setupArduinoPressureSensor(timers);
  setupArduinoTempSensor(timers);
}
