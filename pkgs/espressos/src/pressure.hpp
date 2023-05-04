#pragma once

#include "result.hpp"
#include <Arduino.h>
#include <cstdint>

enum PressureSensorError { OUT_OF_BOUNDS };

typedef Result<int, PressureSensorError> PressureSensorResultRaw_t;
typedef Result<uint16_t, PressureSensorError> PressureSensorResult_t;

class PressureSensor {
private:
  int inputPin;
  std::uint16_t sensorMBar;
  std::uint16_t floor, minFloor, ceil, maxCeil;

public:
  PressureSensor(int inputPin, uint16_t sensorMBar, float sensorRange) {
    this->inputPin = inputPin;
    this->sensorMBar = sensorMBar;

    // Calculate the usable range of values from the ADC
    // A value of 0.8 means that we "lose" 20% of ADC range
    // as the sensor has a voltage range from 0.5v to 4.5v with a VCC of 5v.
    floor = (1024 - (1024 * sensorRange)) / 2;
    ceil = 1024 - floor;

    // We can calculate the floor nicely, but in reality these values have some
    // tolerances so it's best to allow for some runtime recalibration of the sensor range.
    //
    // This sets the tolerance to the floor minus 10%, it Works For Me™, YMMV.
    minFloor = floor * 0.9;
    maxCeil = ceil * 1.1;
  };

  PressureSensorResultRaw_t ReadRaw() {
    PressureSensorResultRaw_t result;

    int value = analogRead(inputPin);

    // Runtime recalibrate the floor if within allowed recalibration range
    if (value < floor && value >= minFloor) {
      floor = value;
    } else if (value > ceil && value <= maxCeil) {
      ceil = value;
    }

    if (value < floor || value > ceil) {
      result.setError(PressureSensorError::OUT_OF_BOUNDS);
    } else {
      result.setValue(value);
    }

    return result;
  };

  PressureSensorResult_t Read() {
    PressureSensorResult_t result;

    auto raw = ReadRaw();
    if (raw.hasError()) {
      result.setError(raw.getError());
    } else {
      result.setValue(MBar(raw.getValue()));
    }

    return result;
  }

  uint16_t MBar(int raw) { return (raw - floor) * sensorMBar / ceil; };
};
