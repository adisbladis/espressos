#include "pressure.hpp"
#include <Arduino.h>

PressureSensor::PressureSensor(int inputPin, float sensorBar,
                               float sensorRange) {
  this->inputPin = inputPin;
  this->sensorBar = sensorBar;

  // Calculate the usable range of values from the ADC
  // A value of 0.8 means that we "lose" 20% of ADC range
  // as the sensor has a voltage range from 0.5v to 4.5v with a VCC of 5v.
  floor = (1024 - (1024 * sensorRange)) / 2;
  ceil = 1024 - floor;
}

int PressureSensor::ReadRaw() { return analogRead(inputPin); }

float PressureSensor::Bar(int raw) { return (raw - floor) * sensorBar / ceil; }

float PressureSensor::Read() { return Bar(ReadRaw()); }
