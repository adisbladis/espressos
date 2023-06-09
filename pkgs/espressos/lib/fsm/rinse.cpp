#include <tinyfsm.hpp>

#include <EventLoop.hpp>
#include <Timers.hpp>

#include "events.hpp"
#include "fsmlist.hpp"
#include "rinse.hpp"
#include "signals.hpp"

class RinseActive;

// The resting state, i.e. before/after a rinse.
class RinseDone : public RinseState {
  void react(RinseStartingEvent const &e) override { transit<RinseActive>(); }
};

// The active state, during the rinse.
class RinseActive : public RinseState {
  void entry() override {
    ::MachineSignals::pump = (PumpTarget){PumpMode::POWER, PumpMax};
    ::MachineSignals::solenoid = true;
    timeout = getEventLoop().setTimeout(RINSE_DUTY_CYCLE,
                                        []() { send_event(RinseStopEvent()); });
  }

  void exit() override {
    ::MachineSignals::solenoid = false;
    ::MachineSignals::pump = (PumpTarget){PumpMode::POWER, PumpMin};
    timeout->cancel();
  }

protected:
  // Time when to transition to RinseDone
  static Timeout_t timeout;
};

void RinseState::react(RinseStoppingEvent const &e) { transit<RinseDone>(); }

Timeout_t RinseActive::timeout = getEventLoop().setTimeout(0, []() {});

FSM_INITIAL_STATE(RinseState, RinseDone)
