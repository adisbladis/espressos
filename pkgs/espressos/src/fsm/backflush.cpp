#include <tinyfsm.hpp>

#include "backflush.hpp"
#include "events.hpp"
#include "fsmlist.hpp"
#include "signals.hpp"

class BackflushActive;
class BackflushResting;

// The resting state, i.e. before/after a backflush
class BackflushDone : public BackflushState {
  void entry() override {
    activeCount = BACKFLUSH_ACTIVE_COUNT;
    ::MachineSignals::pump = (PumpTarget){PumpMode::POWER, PumpMin};
  }

  void react(BackflushStartEvent const &e) override {
    transit<BackflushActive>();
  }
};

// The active state, pump and solenoid on
class BackflushActive : public BackflushState {
  void entry() override {
    timeout = ::MachineSignals::timestamp.get() + BACKFLUSH_DUTY_CYCLE;
    activeCount--;

    if (activeCount == 0) {
      send_event(BackflushStopEvent());
    }

    ::MachineSignals::pump = (PumpTarget){PumpMode::POWER, PumpMax};
  }

  void react(LoopEvent const &e) override {
    if (e.timestamp >= timeout) {
      transit<BackflushResting>();
    }
  }

protected:
  static unsigned long timeout;
};

class BackflushResting : public BackflushState {
  void entry() override {
    timeout = ::MachineSignals::timestamp.get() + (BACKFLUSH_DUTY_CYCLE * 2);
    ::MachineSignals::pump = (PumpTarget){PumpMode::POWER, PumpMin};
  }

  void react(LoopEvent const &e) override {
    if (e.timestamp >= timeout) {
      transit<BackflushActive>();
    }
  }

protected:
  static unsigned long timeout;
};

void BackflushState::react(BackflushStopEvent const &e) {
  transit<BackflushDone>();
}

int BackflushState::activeCount = BACKFLUSH_ACTIVE_COUNT;
unsigned long BackflushActive::timeout = 0;
unsigned long BackflushResting::timeout = 0;

FSM_INITIAL_STATE(BackflushState, BackflushDone)
