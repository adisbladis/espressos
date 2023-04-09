#pragma once

class PressureSensor {
private:
  int inputPin;
  float sensorBar;
  float floor, ceil;

public:
  PressureSensor(int inputPin, float sensorBar, float sensorRange);
  // TODO: Return wrapper taking hardware errors into account
  int ReadRaw();
  float Bar(int raw);
  float Read();
};
