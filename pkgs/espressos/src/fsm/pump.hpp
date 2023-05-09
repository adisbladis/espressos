#pragma once

#include <cstdint>

enum PumpMode { POWER };

static constexpr uint16_t PumpMax = 65535;
static constexpr uint16_t PumpMin = 0;

// Describes the desired state of a pump
struct PumpTarget {
public:
  PumpMode mode;

  // The value carries a different meaning per mode:
  // - POWER
  //   A value between 0 and 65535 expressing the power.
  uint16_t value;

  bool operator==(PumpTarget const &rhs) const {
    return mode == rhs.mode && value == rhs.value;
  }

  bool operator!=(PumpTarget const &rhs) const { return !(*this == rhs); }
};
