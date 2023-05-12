#pragma once

#include <cstdint>

// A bi-state output (can be either on or off)
class ToggleOutput {
public:
  virtual void setup(){};
  virtual void set(bool){};
};

// An output ranging from 0 to 255
class RangeOutput {
public:
  virtual void setup(){};
  virtual void set(std::uint8_t){};
};
