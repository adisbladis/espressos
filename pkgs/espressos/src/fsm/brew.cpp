#include <Arduino.h>
#include <tinyfsm.hpp>

#include "../logger.hpp"
#include "brew.hpp"
#include "events.hpp"
#include "fsmlist.hpp"

// The resting state, i.e. before/after a brew
class BrewDone : public BrewState {
  void entry() override {}
  void exit() override {}

  void react(BrewStartEvent const &e) override {
    logger->log(LogLevel::DEBUG, "Starting brew");
    transit<BrewDone>();
  }
};

FSM_INITIAL_STATE(BrewState, BrewDone)
