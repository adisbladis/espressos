#pragma once

class PressureSensor {
private:
  int inputPin;
  float sensorBar;
  float floor, ceil;

public:
  PressureSensor(int inputPin, float sensorBar, float sensorRange);
  int ReadRaw();
  float Bar(int raw);
  float Read();
};
