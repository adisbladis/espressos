#pragma once

#include "../lib/timers.hpp"

#ifdef ARDUINO
#include "arduino/setup.hpp"
#endif

void setupHAL(Timers &timers) {
#ifdef ARDUINO
  setupArduinoHAL(timers);
#endif
};

void setupAPI(APIHandler &handler, Timers &timers, PersistedConfig &pConfig,
              StateUpdateMessage_t &stateUpdateMessage) {
#ifdef ARDUINO
  setupArduinoAPI(handler, timers, pConfig, stateUpdateMessage);
#endif
}
