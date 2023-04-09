#pragma once

#include <tinyfsm.hpp>

/* Forward declarations */
class Idle;
class Panic;
class Brewing;
class Pumping;
class Steaming;

/* Events */
struct PowerOnEvent : tinyfsm::Event {};
struct PowerOffEvent : tinyfsm::Event {};
struct StartBrewEvent : tinyfsm::Event {};
struct StopBrewEvent : tinyfsm::Event {};
struct StartPumpEvent : tinyfsm::Event {};
struct StopPumpEvent : tinyfsm::Event {};
struct StartSteamEvent : tinyfsm::Event {};
struct StopSteamEvent : tinyfsm::Event {};

/* FSM base class */
class MachineState : public tinyfsm::Fsm<MachineState> {
  friend class tinyfsm::Fsm<MachineState>;

  // Reactions
  void react(tinyfsm::Event const &) { Serial.println("Got unhandled event"); };
  void react(PowerOffEvent const &);
  virtual void react(PowerOnEvent const &) {
    Serial.println("PowerOnEvent ignored");
  };
  virtual void react(StartBrewEvent const &) {
    Serial.println("StartBrewEvent ignored");
  };
  virtual void react(StopBrewEvent const &) {
    Serial.println("StopBrewEvent ignored");
  };
  virtual void react(StartPumpEvent const &) {
    Serial.println("StartPumpEvent ignored");
  };
  virtual void react(StopPumpEvent const &) {
    Serial.println("StopPumpEvent ignored");
  };
  virtual void react(StartSteamEvent const &) {
    Serial.println("StartSteamEvent ignored");
  };
  virtual void react(StopSteamEvent const &) {
    Serial.println("StopSteamEvent ignored");
  };

  virtual void entry(void){}; // entry actions in some states
  void exit(void){};          // no exit actions
};

/* Machine states */

// Panic (i.e. on hardware sensor errors) is exactly like the Off
// state but without a transition to other states
class Panic : public MachineState {
  void entry() override { Serial.println("Entering panic state"); }
};

// The machine is idle with the boiler turned off
class Off : public Panic {
  void entry() override { Serial.println("Entering off state"); };

  void react(PowerOnEvent const &e) override { transit<Idle>(); }
};

class Idle : public MachineState {
  void entry() override { Serial.println("Entering idle state"); };

  void react(StartBrewEvent const &e) override {
    Serial.println("Transitioning to start brew");
    transit<Brewing>();
  }

  void react(StartPumpEvent const &e) override { transit<Pumping>(); }

  void react(StartSteamEvent const &e) override { transit<Steaming>(); }
};

class Brewing : public MachineState {
  void entry() { Serial.println("Transitioned to start brew"); }

  void react(StopBrewEvent const &e) override { transit<Idle>(); }
};

class Pumping : public MachineState {
  void entry() override { Serial.println("Entering pumping state"); }

  void react(StopPumpEvent const &e) override { transit<Idle>(); }
};

class Steaming : public MachineState {
  void entry() override { Serial.println("Entering steaming state"); }

  void react(StopSteamEvent const &e) override { transit<Idle>(); }
};

/* Shared class methods*/

// Only power off if we're not in panic mode
void MachineState::react(PowerOffEvent const &e) {
  if (is_in_state<Panic>()) {
    // TODO: Emit error message
    return;
  }

  transit<Off>();
}

/* Initial state */
FSM_INITIAL_STATE(MachineState, Off)
