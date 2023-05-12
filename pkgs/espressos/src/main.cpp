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

// Watch variables for change and propagate to hardware/API
static Effects effects;
static Effects apiEffects;
static Timers timers;
static Timers apiTimers;
static Loops apiLoops;

// Re-use loop event on every iteration
static LoopEvent loopEvent;
static TimeEvent timeEvent;

static StateUpdateMessage_t stateUpdateMessage;

void setup() {
  // Set up logging
  Serial.begin(115200);

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

  // Set API server states
  {
    static PersistedConfig pConfig;
    static APIHandler apiHandler(&pConfig);

    pConfig.setup();
    setupAPI(apiHandler, apiTimers, apiEffects, apiLoops, pConfig,
             stateUpdateMessage);
    setupAPIEffects(apiEffects, stateUpdateMessage);

    apiEffects.createEffect<unsigned long>(
        []() { return MachineState::current_state_ptr->getTimestamp(); },
        [](unsigned long now) { apiTimers.loop(now); }, false);
  }
}

void loop() {
  // Run FSM & reconcile hardware with FSM state
  effects.loop();

  // Set API states
  apiEffects.loop();

  apiLoops.loop();
}
