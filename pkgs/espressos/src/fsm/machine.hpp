#pragma once

#include "../logger.hpp"
#include <tinyfsm.hpp>

/* Forward declarations */
class Idle;
class Panic;
class Brewing;
class Pumping;
class Steaming;

/* Events */
struct PowerOnEvent : tinyfsm::Event {
  int setpoint;
};
struct StartSteamEvent : tinyfsm::Event {
  int setpoint;
};

struct PanicEvent : tinyfsm::Event {};
struct PowerOffEvent : tinyfsm::Event {};
struct StartBrewEvent : tinyfsm::Event {};
struct StopBrewEvent : tinyfsm::Event {};
struct StartPumpEvent : tinyfsm::Event {};
struct StopPumpEvent : tinyfsm::Event {};
struct StopSteamEvent : tinyfsm::Event {};

/* FSM base class */
class MachineState : public tinyfsm::Fsm<MachineState> {
  friend class tinyfsm::Fsm<MachineState>;

  // Reactions
  void react(tinyfsm::Event const &) {
    logger->log(LogLevel::DEBUG, "Got unhandled event");
  };
  void react(PanicEvent const &);
  void react(PowerOffEvent const &);
  virtual void react(PowerOnEvent const &) {
    logger->log(LogLevel::DEBUG, "PowerOnEvent ignored");
  };
  virtual void react(StartBrewEvent const &) {
    logger->log(LogLevel::DEBUG, "StartBrewEvent ignored");
  };
  virtual void react(StopBrewEvent const &) {
    logger->log(LogLevel::DEBUG, "StopBrewEvent ignored");
  };
  virtual void react(StartPumpEvent const &) {
    logger->log(LogLevel::DEBUG, "StartPumpEvent ignored");
  };
  virtual void react(StopPumpEvent const &) {
    logger->log(LogLevel::DEBUG, "StopPumpEvent ignored");
  };
  virtual void react(StartSteamEvent const &) {
    logger->log(LogLevel::DEBUG, "StartSteamEvent ignored");
  };
  virtual void react(StopSteamEvent const &) {
    logger->log(LogLevel::DEBUG, "StopSteamEvent ignored");
  };

  virtual void entry(void){}; // entry actions in some states
  virtual void exit(void){};  // exit actions in some states

protected:
  static int steamSetpoint;
  static int setpoint;

public:
  virtual int getSetPoint() { return setpoint; };
};

/* Machine states */

// Panic (i.e. on hardware sensor errors) is exactly like the Off
// state but without a transition to other states
class Panic : public MachineState {
  void entry() override {
    logger->log(LogLevel::ERROR, "Entering panic state");
    setpoint = 0;
  }
};

// The machine is idle with the boiler turned off
class Off : public MachineState {
  void entry() override {
    logger->log(LogLevel::DEBUG, "Entering off state");
    setpoint = 0;
  };

  void react(PowerOnEvent const &e) override {
    setpoint = e.setpoint;
    transit<Idle>();
  }
};

class Idle : public MachineState {
  void entry() override {
    logger->log(LogLevel::DEBUG, "Entering idle state");
  };

  void react(PowerOnEvent const &e) override {
    setpoint = e.setpoint;
    transit<Idle>();
  }

  void react(StartBrewEvent const &e) override {
    logger->log(LogLevel::DEBUG, "Transitioning to start brew");
    transit<Brewing>();
  }

  void react(StartPumpEvent const &e) override { transit<Pumping>(); }

  void react(StartSteamEvent const &e) override {
    steamSetpoint = e.setpoint;
    transit<Steaming>();
  }
};

class Brewing : public MachineState {
  void entry() { logger->log(LogLevel::DEBUG, "Transitioned to start brew"); }

  void react(StopBrewEvent const &e) override { transit<Idle>(); }
};

class Pumping : public MachineState {
  void entry() override {
    logger->log(LogLevel::DEBUG, "Entering pumping state");
  }

  void react(StopPumpEvent const &e) override { transit<Idle>(); }
};

class Steaming : public MachineState {
  void entry() override {
    logger->log(LogLevel::DEBUG, "Entering steaming state");
  }

  void exit() override { steamSetpoint = 0; };

  void react(StopSteamEvent const &e) override { transit<Idle>(); }

  int getSetPoint() { return steamSetpoint; };
};

/* Shared class methods*/

// Only power off if we're not in panic mode
void MachineState::react(PowerOffEvent const &e) {
  if (is_in_state<Panic>()) {
    return;
  }

  transit<Off>();
}

// All modes are allowed to enter the panic state
void MachineState::react(PanicEvent const &e) {
  if (is_in_state<Panic>()) {
    return;
  }

  transit<Panic>();
}

int MachineState::setpoint = 0;
int MachineState::steamSetpoint = 0;

/* Initial state */
FSM_INITIAL_STATE(MachineState, Off)
