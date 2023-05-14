#include <tinyfsm.hpp>

#include "events.hpp"
#include "fsmlist.hpp"
#include "rinse.hpp"
#include "signals.hpp"

class RinseActive;

// The resting state, i.e. before/after a rinse
class RinseDone : public RinseState {
  void entry() override {
    ::MachineSignals::solenoid = false;
    ::MachineSignals::pump = (PumpTarget){PumpMode::POWER, PumpMin};
  }

  void react(RinseStartEvent const &e) override {
    timeout = ::MachineSignals::timestamp.get() + RINSE_DUTY_CYCLE;
    transit<RinseActive>();
  }
};

// The active state, pump and solenoid on
class RinseActive : public RinseState {
  void entry() override {
    ::MachineSignals::pump = (PumpTarget){PumpMode::POWER, PumpMax};
    ::MachineSignals::solenoid = true;
  }

  void react(LoopEvent const &e) override {
    if (e.timestamp >= timeout) {
      send_event(RinseStopEvent());
    }
  }

public:
  bool active() override { return true; }
};

void RinseState::react(RinseStopEvent const &e) { transit<RinseDone>(); }

unsigned long RinseState::timeout = 0;

FSM_INITIAL_STATE(RinseState, RinseDone)
