#include <tinyfsm.hpp>

#include <EventLoop.hpp>

#include "backflush.hpp"
#include "events.hpp"
#include "fsmlist.hpp"
#include "signals.hpp"

class BackflushActive;
class BackflushResting;

// The resting state, i.e. before/after a backflush
class BackflushDone : public BackflushState {
  void entry() override { activeCount = BACKFLUSH_ACTIVE_COUNT; }

  void react(BackflushStartingEvent const &e) override {
    transit<BackflushActive>();
  }
};

// The active state, pump and solenoid on
class BackflushActive : public BackflushState {
  void entry() override {
    if (activeCount == 0) {
      send_event(BackflushStopEvent());
      return;
    }
    activeCount--;

    timeout = getEventLoop().setTimeout(
        BACKFLUSH_DUTY_CYCLE, []() { send_event(BackflushDeactivateEvent()); });

    ::MachineSignals::pump = (PumpTarget){PumpMode::POWER, PumpMax};
    ::MachineSignals::solenoid = true;
  }

  void react(BackflushDeactivateEvent const &) override {
    transit<BackflushResting>();
  }

  void exit() override {
    ::MachineSignals::pump = (PumpTarget){PumpMode::POWER, PumpMin};
    ::MachineSignals::solenoid = false;
    timeout->cancel();
  }

protected:
  static Timeout_t timeout;
};

class BackflushResting : public BackflushState {
  void entry() override {
    if (activeCount == 0) {
      send_event(BackflushStopEvent());
      return;
    }

    activeCount--;
    timeout = getEventLoop().setTimeout(BACKFLUSH_DUTY_CYCLE * 2, []() {
      send_event(BackflushActivateEvent());
    });
  }

  void exit() override { timeout->cancel(); }

  void react(BackflushActivateEvent const &) override {
    transit<BackflushActive>();
  }

protected:
  static Timeout_t timeout;
};

void BackflushState::react(BackflushStoppingEvent const &e) {
  transit<BackflushDone>();
}

int BackflushState::activeCount = BACKFLUSH_ACTIVE_COUNT;
Timeout_t BackflushActive::timeout = getEventLoop().setTimeout(0, []() {});
Timeout_t BackflushResting::timeout = getEventLoop().setTimeout(0, []() {});

FSM_INITIAL_STATE(BackflushState, BackflushDone)
