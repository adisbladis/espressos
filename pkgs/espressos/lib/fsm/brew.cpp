#include <tinyfsm.hpp>

#include <Time.hpp>

#include "brew.hpp"
#include "events.hpp"
#include "fsmlist.hpp"
#include "signals.hpp"

class BrewActive;

// The resting state, i.e. before/after a brew
class BrewDone : public BrewState {
  void react(BrewStartingEvent const &e) override { transit<BrewActive>(); }
};

// While brewing
class BrewActive : public BrewState {
  void entry() override {
    ::MachineSignals::shotStartTime = millis();
    ::MachineSignals::solenoid = true;
  };

  void exit() override {
    ::MachineSignals::shotStopTime = millis();
    ::MachineSignals::solenoid = false;
    ::MachineSignals::pump = (PumpTarget){PumpMode::POWER, PumpMin};
  };

  // Only allow setting target pressure while we're actually brewing.
  void react(BrewTargetEvent const &e) override {
    PumpTarget pumpTarget;
    pumpTarget.value = e.value;

    // Set the pump target mode according to the event mode
    switch (e.mode) {
    case BrewStateMode::POWER:
      pumpTarget.mode = PumpMode::POWER;
      break;

    case BrewStateMode::PRESSURE:
      pumpTarget.mode = PumpMode::PRESSURE;
      break;

    default:
      send_event(PanicEvent("Unknown brew state mode"));
      return;
    };

    ::MachineSignals::pump = pumpTarget;
  }
};

// We can stop brewing from all states
void BrewState::react(BrewStoppingEvent const &e) { transit<BrewDone>(); };

FSM_INITIAL_STATE(BrewState, BrewDone)
