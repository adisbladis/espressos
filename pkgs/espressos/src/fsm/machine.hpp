#pragma once

#include <tinyfsm.hpp>

#include "../logger.hpp"
#include "events.hpp"

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
  // Boiler setpoint
  static int setpoint;

  // Store the previous boiler setpoint when entering steam mode
  // so we can easily transition back into idle mode with the correct setpoint.
  static int prevSetpoint;

  // How often to emit state updates to clients
  static long stateUpdateInterval;

public:
  int getSetPoint() { return setpoint; };
  long getStateUpdateInterval() { return stateUpdateInterval; };
  virtual PinStatus getSolenoid() { return LOW; }
  virtual uint8_t getPump() { return 0; }
};

/* Machine states */

// Panic (i.e. on hardware sensor errors) is exactly like the Off
// state but without a transition to other states
class Panic : public MachineState {
  void entry() override { setpoint = 0; }
};

// The machine is idle with the boiler turned off
class Off : public MachineState {
  void entry() override { setpoint = 0; };

  void react(PowerOnEvent const &e) override {
    setpoint = e.setpoint;
    logger->log(LogLevel::DEBUG, "Entering idle state");
    transit<Idle>();
  }
};

class Idle : public MachineState {
  void react(PowerOnEvent const &e) override {
    setpoint = e.setpoint;
    transit<Idle>();
  }

  void react(StartBrewEvent const &e) override {
    logger->log(LogLevel::DEBUG, "Entering brew state");
    transit<Brewing>();
  }

  void react(StartPumpEvent const &e) override {
    logger->log(LogLevel::DEBUG, "Entering pumping state");
    transit<Pumping>();
  }

  void react(StartSteamEvent const &e) override {
    prevSetpoint = setpoint;
    setpoint = e.setpoint;
    logger->log(LogLevel::DEBUG, "Entering steaming state");
    transit<Steaming>();
  }
};

class Brewing : public MachineState {
  void entry() { stateUpdateInterval = STATE_UPDATE_INTERVAL_BREW; }

  void exit() override { stateUpdateInterval = STATE_UPDATE_INTERVAL; };

  void react(StopBrewEvent const &e) override { transit<Idle>(); }

  PinStatus getSolenoid() override { return HIGH; }

  uint8_t getPump() override { return 255; }
};

class Pumping : public MachineState {
  void react(StopPumpEvent const &e) override { transit<Idle>(); }
  uint8_t getPump() override { return 255; }
};

class Steaming : public MachineState {
  void exit() override { setpoint = prevSetpoint; };

  void react(StopSteamEvent const &e) override { transit<Idle>(); }
};

/* Shared class methods*/

// Only power off if we're not in panic mode
void MachineState::react(PowerOffEvent const &e) {
  if (is_in_state<Panic>()) {
    return;
  }

  logger->log(LogLevel::DEBUG, "Entering off state");

  transit<Off>();
}

// All modes are allowed to enter the panic state
void MachineState::react(PanicEvent const &e) {
  logger->log(LogLevel::ERROR, "Entering panic state");
  transit<Panic>();
}

int MachineState::setpoint = 0;
int MachineState::prevSetpoint = 0;
long MachineState::stateUpdateInterval = STATE_UPDATE_INTERVAL;

/* Initial state */
FSM_INITIAL_STATE(MachineState, Off)
