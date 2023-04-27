#include <Arduino.h>
#include <tinyfsm.hpp>

#include "../logger.hpp"
#include "backflush.hpp"
#include "events.hpp"
#include "fsmlist.hpp"

class BackflushActive;
class BackflushResting;

// The resting state, i.e. before/after a backflush
class BackflushDone : public BackflushState {
  void entry() override { activeCount = BACKFLUSH_ACTIVE_COUNT; }

  void react(BackflushStartEvent const &e) override {
    logger->log(LogLevel::DEBUG, "Activating backflush");
    transit<BackflushActive>();
  }
};

// The active state, pump and solenoid on
class BackflushActive : public BackflushState {
  void entry() override {
    timeout = millis() + BACKFLUSH_DUTY_CYCLE;
    activeCount--;

    if (activeCount == 0) {
      send_event(BackflushStopEvent());
    }
  }

  void react(LoopEvent const &e) override {
    if (e.timestamp >= timeout) {
      logger->log(LogLevel::DEBUG,
                  "Backflush active timed out, transitition back to resting.");
      transit<BackflushResting>();
    }
  }

protected:
  static unsigned long timeout;

public:
  bool active() override { return true; }
};

class BackflushResting : public BackflushState {
  void entry() override { timeout = millis() + (BACKFLUSH_DUTY_CYCLE * 2); }

  void react(LoopEvent const &e) override {
    if (e.timestamp >= timeout) {
      logger->log(LogLevel::DEBUG,
                  "Backflush resting timed out, transitition back to active.");
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
