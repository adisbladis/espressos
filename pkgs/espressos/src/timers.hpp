#pragma once

#include "lib/timers.hpp"

// The global timers singleton
inline Timers timers;

// A no-op function to pass to timeouts as a default value on initialisation
inline void DummyFunc() {};
