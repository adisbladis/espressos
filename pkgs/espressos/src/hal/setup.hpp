#pragma once

#include "../lib/timers.hpp"
#include "time.hpp"

#ifdef ARDUINO
#include "arduino/setup.hpp"
#endif

void setupHAL(Timers &timers) {
#ifdef ARDUINO
  setupArduinoHAL(timers);
#endif
};

void setupAPI(APIHandler &handler, Timers &timers,
              StateUpdateMessage_t &stateUpdateMessage) {
#ifdef ARDUINO
  setupArduinoAPI(handler, timers, stateUpdateMessage);
#endif
}
