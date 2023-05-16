#pragma once

#include <cstdint>
#include <tinyfsm.hpp>

#include "../hal/time.hpp"

// When the machine pressure changes a pressure event is emitted
struct PressureEvent : tinyfsm::Event {
  uint16_t pressure;
};

struct TempEvent : tinyfsm::Event {
  int16_t temp;
};

struct BackflushStartEvent : tinyfsm::Event {};
struct BackflushStopEvent : tinyfsm::Event {};

struct BackflushActivateEvent : tinyfsm::Event {};
struct BackflushDeactivateEvent : tinyfsm::Event {};

struct BrewStartEvent : tinyfsm::Event {};
struct BrewStopEvent : tinyfsm::Event {};

enum struct BrewTargetEventMode { POWER, PRESSURE };
struct BrewTargetEvent : tinyfsm::Event {
  BrewTargetEventMode mode;
  uint16_t value;

  bool operator==(BrewTargetEvent const &rhs) const {
    return mode == rhs.mode && value == rhs.value;
  }

  bool operator!=(BrewTargetEvent const &rhs) const { return !(*this == rhs); }
};
