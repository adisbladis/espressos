#include <tinyfsm.hpp>

#include "brew.hpp"
#include "events.hpp"
#include "fsmlist.hpp"
#include "pump.hpp"
#include "signals.hpp"

class BrewActive;

// The resting state, i.e. before/after a brew
class BrewDone : public BrewState {
  void entry() override {
    ::MachineSignals::pump = (PumpTarget){PumpMode::POWER, 0};
  }

  void react(BrewStartEvent const &e) override {
    ::MachineSignals::shotStartTime = ::MachineSignals::timestamp.get();
    ::MachineSignals::shotStartTime = 0;
    transit<BrewActive>();
  }
};

// While brewing
class BrewActive : public BrewState {

  // Only allow setting target pressure while we're actually brewing.
  void react(BrewTargetEvent const &e) override {
    PumpTarget pumpTarget;
    pumpTarget.value = e.value;

    switch (e.mode) {
    case BrewStateMode::POWER:
      pumpTarget.mode = PumpMode::POWER;
    case BrewStateMode::PRESSURE:
      pumpTarget.mode = PumpMode::PRESSURE;
    };

    ::MachineSignals::pump = pumpTarget;
  }
};

// We can stop brewing from all states
void BrewState::react(BrewStopEvent const &e) {
  ::MachineSignals::shotStopTime = ::MachineSignals::timestamp.get();
  transit<BrewDone>();
};

FSM_INITIAL_STATE(BrewState, BrewDone)
