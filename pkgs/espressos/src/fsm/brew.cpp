#include <tinyfsm.hpp>

#include "brew.hpp"
#include "events.hpp"
#include "fsmlist.hpp"

class BrewActive;

// The resting state, i.e. before/after a brew
class BrewDone : public BrewState {
  void react(BrewStartEvent const &e) override {
    shotStartTime = timestamp;
    shotStopTime = 0;
    transit<BrewActive>();
  }
};

// While brewing
class BrewActive : public BrewState {};

// We can stop brewing from all states
void BrewState::react(BrewStopEvent const &e) {
  shotStopTime = timestamp;
  transit<BrewDone>();
};

unsigned long BrewState::shotStartTime = 0;
unsigned long BrewState::shotStopTime = 0;
unsigned long BrewState::timestamp = 0;

FSM_INITIAL_STATE(BrewState, BrewDone)
