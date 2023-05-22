#pragma once

// Use millis() on Arduino with unsigned long type
#ifdef ARDUINO
#include <Arduino.h>
typedef unsigned long Timestamp_t;
#else
#include <chrono>
#include <cstdint>
typedef uint64_t Timestamp_t;

inline Timestamp_t millis() {
  using namespace std::chrono;
  return duration_cast<milliseconds>(system_clock::now().time_since_epoch())
      .count();
}
#endif
