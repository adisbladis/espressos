#pragma once

#include <cstdint>
#include <tinyfsm.hpp>

#include "../hal/time.hpp"

// This behaves exactly the same as the LoopEvent but the intent differs.
// The intent for this event is for state machines that do things in relation
// to the current time to be able to update their internal timestamps.
struct TimeEvent : tinyfsm::Event {
  Timestamp_t timestamp;
};

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
