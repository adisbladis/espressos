#pragma once

#include <Arduino.h>

#include "../io.hpp"

// Implements a toggling output using the Arduino function digitalWrite
class ArduinoToggleOutput : public ToggleOutput {
private:
  int pin;

public:
  ArduinoToggleOutput(int pin) : pin(pin){};

  void setup() override { pinMode(pin, OUTPUT); };

  void set(const bool value) override { digitalWrite(pin, value); };
};
