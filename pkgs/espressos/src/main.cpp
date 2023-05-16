#include <cstdint>

#include "api/effects.hpp"
#include "api/handler.hpp"
#include "api/types.hpp"
#include "fsm/fsmlist.hpp"
#include "hal/setup.hpp"
#include "lib/timers.hpp"
#include "timers.hpp"

void setup() {
  {
    // Initialise the FSM
    fsm_list::start();

    setupHAL();
  }

  // Set up user IO
  {
    static StateUpdateMessage_t stateUpdateMessage;

    static APIHandler apiHandler;

    setupAPI(apiHandler, stateUpdateMessage);

    // Set up effects that update the API state
    setupAPIEffects(stateUpdateMessage);
  }
}

void loop() {
  Timestamp_t now = millis();
  timers.loop(now);
}
