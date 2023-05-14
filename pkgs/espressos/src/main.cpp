#include <cstdint>

#include "api/effects.hpp"
#include "api/handler.hpp"
#include "api/types.hpp"
#include "config.hpp"
#include "fsm/fsmlist.hpp"
#include "hal/setup.hpp"
#include "lib/timers.hpp"

static Timers timers;
static Timers apiTimers;

void setup() {
  {
    // Initialise the FSM
    fsm_list::start();

    timers.createInterval(1, [](unsigned long now) {
      static TimeEvent timeEvent;
      timeEvent.timestamp = now;
      send_event(timeEvent);

      static LoopEvent loopEvent;
      loopEvent.timestamp = now;
      send_event(loopEvent);
    });

    setupHAL(timers);
  }

  // Set up user IO
  {
    static StateUpdateMessage_t stateUpdateMessage;

    static PersistedConfig pConfig;
    static APIHandler apiHandler(&pConfig);

    pConfig.setup();
    setupAPI(apiHandler, apiTimers, pConfig, stateUpdateMessage);

    // Set up effects that update the API state
    setupAPIEffects(stateUpdateMessage);
  }
}

void loop() {
  unsigned long now = millis();
  timers.loop(now);
  apiTimers.loop(now);
}
