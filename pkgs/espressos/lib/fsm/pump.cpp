#include <tinyfsm.hpp>

#include <Time.hpp>

#include "brew.hpp"
#include "events.hpp"
#include "fsmlist.hpp"
#include "signals.hpp"

class PumpActive;

// The resting state, i.e. before/after running the pump
class PumpDone : public PumpState {
  void react(PumpStartingEvent const &e) override { transit<PumpActive>(); }
};

// While brewing
class PumpActive : public PumpState {
  void entry() override {
    ::MachineSignals::pump = (PumpTarget){PumpMode::POWER, PumpMax};
  };

  void exit() override {
    ::MachineSignals::pump = (PumpTarget){PumpMode::POWER, PumpMin};
  };
};

// We can stop brewing from all states
void PumpState::react(PumpStoppingEvent const &e) { transit<PumpDone>(); };

FSM_INITIAL_STATE(PumpState, PumpDone)
