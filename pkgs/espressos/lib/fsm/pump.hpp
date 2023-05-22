#pragma once

#include <cstdint>

enum struct PumpMode {
  // In POWER we're controlling the dimmer somewhat directly.
  // It has a range between 0 to 65535 and says how much power we should
  // deliver to the pump.
  POWER,

  // We're pressure profiling
  PRESSURE,
};

static constexpr uint16_t PumpMax = 65535;
static constexpr uint16_t PumpMin = 0;

// Describes the desired state of a pump
struct PumpTarget {
public:
  PumpMode mode;

  // The value carries a different meaning per mode:
  // - POWER
  //   A value between 0 and 65535 expressing the power.
  // - Pressure
  //   The target pressure in millibar.
  uint16_t value;

  bool operator==(PumpTarget const &rhs) const {
    return mode == rhs.mode && value == rhs.value;
  }

  bool operator!=(PumpTarget const &rhs) const { return !(*this == rhs); }
};
