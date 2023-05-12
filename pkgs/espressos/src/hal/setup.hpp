#pragma once

#ifdef ARDUINO
#include "arduino/setup.hpp"
#else
void setupHAL(Timers) { };
#endif
