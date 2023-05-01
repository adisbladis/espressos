#include <Arduino.h>
#include <tinyfsm.hpp>

#include "../logger.hpp"
#include "events.hpp"
#include "fsmlist.hpp"
#include "rinse.hpp"

class RinseActive;

// The resting state, i.e. before/after a rinse
class RinseDone : public RinseState {
  void react(RinseStartEvent const &e) override {
    logger->log(LogLevel::DEBUG, "Activating rinse");
    timeout = e.timestamp + RINSE_DUTY_CYCLE;
    transit<RinseActive>();
  }
};

// The active state, pump and solenoid on
class RinseActive : public RinseState {
  void react(LoopEvent const &e) override {
    if (e.timestamp >= timeout) {
      logger->log(LogLevel::DEBUG,
                  "Rinse active timed out, transitition back to done.");
      send_event(RinseStopEvent());
    }
  }

public:
  bool active() override { return true; }
};

void RinseState::react(RinseStopEvent const &e) { transit<RinseDone>(); }

unsigned long RinseState::timeout = 0;

FSM_INITIAL_STATE(RinseState, RinseDone)
