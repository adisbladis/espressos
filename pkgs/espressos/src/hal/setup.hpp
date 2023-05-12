#pragma once

#include "../lib/effects.hpp"
#include "../lib/timers.hpp"

#ifdef ARDUINO
#include "arduino/setup.hpp"
#else
void setupHAL(Timers, Effects){};
#endif
