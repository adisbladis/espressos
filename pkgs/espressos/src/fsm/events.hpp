#pragma once

#include <tinyfsm.hpp>

struct LoopEvent : tinyfsm::Event {
  unsigned long timestamp;
};

struct BackflushStartEvent : tinyfsm::Event {};
struct BackflushStopEvent : tinyfsm::Event {};
