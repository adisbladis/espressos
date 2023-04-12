#pragma once
#include <Arduino.h>

// A digitalWrite/digitalRead pin wrapper that caches the current state.
class CachedOutputPin {
private:
  uint8_t pin;
  PinStatus val;

public:
  CachedOutputPin(uint8_t pin) : CachedOutputPin(pin, LOW){};
  CachedOutputPin(uint8_t pin, PinStatus val) : pin(pin), val(val){};

  void setup() {
    pinMode(pin, OUTPUT);
    digitalWrite(val);
  }

  uint8_t getPin() { return pin; }

  // Simply return the cached value
  PinStatus digitalRead() { return val; }

  // Write and cache the value, return if value is the same
  void digitalWrite(PinStatus value) {
    if (val == value) {
      return;
    }

    val = value;
    ::digitalWrite(pin, value);
  }
};
