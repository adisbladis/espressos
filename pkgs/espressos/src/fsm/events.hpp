#pragma once

#include <tinyfsm.hpp>

struct LoopEvent : tinyfsm::Event {
  unsigned long timestamp;
};
