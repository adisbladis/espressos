#pragma once

#include "../lib/effects.hpp"
#include "../lib/timers.hpp"

#ifdef ARDUINO
#include "arduino/setup.hpp"
#endif

void setupHAL(Timers &timers, Effects &effects) {
#ifdef ARDUINO
  setupArduinoHAL(timers, effects);
#endif
};

void setupAPI(APIHandler &handler, Timers &timers, Effects &effects,
              PersistedConfig &pConfig,
              StateUpdateMessage_t &stateUpdateMessage) {
#ifdef ARDUINO
  setupArduinoAPI(handler, timers, effects, pConfig, stateUpdateMessage);
#endif
}
