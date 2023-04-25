#pragma once

#include "result.hpp"
#include <Arduino.h>

enum PressureSensorError { OUT_OF_BOUNDS };

typedef Result<int, PressureSensorError> PressureSensorResultRaw_t;
typedef Result<float, PressureSensorError> PressureSensorResult_t;

class PressureSensor {
private:
  int inputPin;
  float sensorBar;
  float floor, ceil;

public:
  PressureSensor(int inputPin, float sensorBar, float sensorRange) {
    this->inputPin = inputPin;
    this->sensorBar = sensorBar;

    // Calculate the usable range of values from the ADC
    // A value of 0.8 means that we "lose" 20% of ADC range
    // as the sensor has a voltage range from 0.5v to 4.5v with a VCC of 5v.
    floor = (1024 - (1024 * sensorRange)) / 2;
    ceil = 1024 - floor;
  };
  // TODO: Return wrapper taking hardware errors into account
  PressureSensorResultRaw_t ReadRaw() {
    PressureSensorResultRaw_t result;

    int value = analogRead(inputPin);
    if (value < 10 || value > (1024 - 10)) {
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
      result.setValue(Bar(raw.getValue()));
    }

    return result;
  }
  float Bar(int raw) { return (raw - floor) * sensorBar / ceil; };
};
