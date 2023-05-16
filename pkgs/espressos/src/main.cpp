#include <cstdint>

#include "api/effects.hpp"
#include "api/handler.hpp"
#include "api/types.hpp"
#include "fsm/fsmlist.hpp"
#include "hal/setup.hpp"
#include "lib/timers.hpp"
#include "timers.hpp"

void setup() {
  // Initialise the FSM
  fsm_list::start();
  setupHAL();

  // Set up user IO
  setupAPI();
}

#ifndef ARDUINO
#include <unistd.h>

// A naive main() implementation for non-arduino platforms
int main() {
  setup();

  while (true) {
    usleep(1000);
    timers.loop(millis());
  }
}

#endif
