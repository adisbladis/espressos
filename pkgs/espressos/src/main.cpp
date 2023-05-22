#include <cstdint>

#include <EventLoop.hpp>
#include <Timers.hpp>
#include <fsmlist.hpp>

#include "api/effects.hpp"
#include "api/handler.hpp"
#include "api/types.hpp"
#include "hal/setup.hpp"

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

  auto eventloop = getEventLoop();

  while (true) {
    usleep(1000);
    eventloop.loop(millis());
  }
}

#endif
