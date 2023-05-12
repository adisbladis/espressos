#include <cstdint>

#include "api/effects.hpp"
#include "api/handler.hpp"
#include "api/types.hpp"
#include "config.hpp"
#include "fsm/brew.hpp"
#include "fsm/events.hpp"
#include "fsm/fsmlist.hpp"
#include "fsm/machine.hpp"
#include "fsm/pump.hpp"
#include "hal/setup.hpp"
#include "lib/effects.hpp"
#include "lib/loops.hpp"
#include "lib/map.hpp"
#include "lib/timers.hpp"
#include "logger.hpp"
#include "proto/api.h"

static Loops loops;
static Loops apiLoops;

void setup() {
  {
    static Effects effects;
    static Timers timers;
    static LoopEvent loopEvent;
    static TimeEvent timeEvent;

    // Initialise the FSM
    fsm_list::start();

    // Run FSM loops on millisecond change
    effects.createEffect<unsigned long>(millis, [](unsigned long now) {
      timeEvent.timestamp = now;
      send_event(timeEvent);

      loopEvent.timestamp = now;
      send_event(loopEvent);

      timers.loop(now);
    });

    setupHAL(timers, effects);

    loops.addFunc([]() { effects.loop(); });
  }

  // Set up user IO
  {
    Serial.begin(115200);

    static Effects apiEffects;
    static Timers apiTimers;
    static StateUpdateMessage_t stateUpdateMessage;

    static PersistedConfig pConfig;
    static APIHandler apiHandler(&pConfig);

    pConfig.setup();
    setupAPI(apiHandler, apiTimers, apiEffects, apiLoops, pConfig,
             stateUpdateMessage);
    setupAPIEffects(apiEffects, stateUpdateMessage);

    apiEffects.createEffect<unsigned long>(
        []() { return MachineState::current_state_ptr->getTimestamp(); },
        [](unsigned long now) { apiTimers.loop(now); }, false);

    apiLoops.addFunc([]() { apiEffects.loop(); });
  }
}

void loop() {
  loops.loop();
  apiLoops.loop();
}
