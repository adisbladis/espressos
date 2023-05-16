#pragma once

#include "time.hpp"

#ifdef ARDUINO
#include "arduino/setup.hpp"
#endif

void setupHAL() {
#ifdef ARDUINO
  setupArduinoHAL();
#endif
};

void setupAPI(APIHandler &handler, StateUpdateMessage_t &stateUpdateMessage) {
#ifdef ARDUINO
  setupArduinoAPI(handler, stateUpdateMessage);
#endif
}
