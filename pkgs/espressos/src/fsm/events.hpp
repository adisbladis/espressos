#pragma once

#include <cstdint>
#include <tinyfsm.hpp>

// This event is dispatcehd with a timestamp on every call to loop()
// and is intended for usage by state machines that needs to react to the time.
struct LoopEvent : tinyfsm::Event {
  unsigned long timestamp;
};

// This behaves exactly the same as the LoopEvent but the intent differs.
// The intent for this event is for state machines that do things in relation
// to the current time to be able to update their internal timestamps.
struct TimeEvent : tinyfsm::Event {
  unsigned long timestamp;
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
