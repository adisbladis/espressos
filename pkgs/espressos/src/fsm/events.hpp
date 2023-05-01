#pragma once

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

struct BackflushStartEvent : tinyfsm::Event {};
struct BackflushStopEvent : tinyfsm::Event {};

struct BrewStartEvent : tinyfsm::Event {};
struct BrewStopEvent : tinyfsm::Event {};
