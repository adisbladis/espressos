#pragma once

#include <Time.hpp>

#ifdef ARDUINO
#include "arduino/setup.hpp"
#endif

void setupHAL() {
#ifdef ARDUINO
  setupArduinoHAL();
#endif
};

void setupAPI() {
#ifdef ARDUINO
  setupArduinoAPI();
#endif
}
