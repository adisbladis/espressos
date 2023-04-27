#pragma once

#include <tinyfsm.hpp>

#include "../logger.hpp"
#include "backflush.hpp"
#include "events.hpp"
#include "fsmlist.hpp"

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
  virtual void react(LoopEvent const &){};
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
  virtual void react(BackflushStartEvent const &) {
    logger->log(LogLevel::DEBUG, "BackflushStartEvent ignored");
  };
  virtual void react(BackflushStopEvent const &) {
    logger->log(LogLevel::DEBUG, "BackflushStopEvent ignored");
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

  // Temporary while we move away from is_in_state
  virtual bool isOn() { return true; }
  virtual bool isBrewing() { return false; }
  virtual bool isPumping() { return false; }
  virtual bool isSteaming() { return false; }
};
