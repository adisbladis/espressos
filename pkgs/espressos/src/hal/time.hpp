#pragma once

// Deal with differences in time

// Use millis() on Arduino with unsigned long type
#ifdef ARDUINO
#include <Arduino.h>
typedef unsigned long Timestamp_t;

// Use std::chrono by default on other platforms with uint64_4 type
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
