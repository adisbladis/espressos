#include <tinyfsm.hpp>

#include "../logger.hpp"
#include "brew.hpp"
#include "events.hpp"
#include "fsmlist.hpp"

class BrewActive;

// The resting state, i.e. before/after a brew
class BrewDone : public BrewState {
  void react(BrewStartEvent const &e) override {
    logger->log(LogLevel::DEBUG, "Starting brew");
    shotStartTime = e.timestamp;
    shotStopTime = 0;
    transit<BrewActive>();
  }
};

// While brewing
class BrewActive : public BrewState {};

// We can stop brewing from all states
void BrewState::react(BrewStopEvent const &e) {
  logger->log(LogLevel::DEBUG, "Transitioning to BrewDone");
  shotStopTime = e.timestamp;
  transit<BrewDone>();
};

unsigned long BrewState::shotStartTime = 0;
unsigned long BrewState::shotStopTime = 0;

FSM_INITIAL_STATE(BrewState, BrewDone)
