#pragma once

#include <tinyfsm.hpp>

#include "../logger.hpp"
#include "../proto/api.h"
#include "backflush.hpp"
#include "events.hpp"
#include "fsmlist.hpp"
#include "rinse.hpp"

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
struct StartPumpEvent : tinyfsm::Event {};
struct StopPumpEvent : tinyfsm::Event {};
struct StopSteamEvent : tinyfsm::Event {};

/* FSM base class */
class MachineState : public tinyfsm::Fsm<MachineState> {
  friend class tinyfsm::Fsm<MachineState>;

  void react(tinyfsm::Event const &){};

  void react(PanicEvent const &);
  void react(PowerOffEvent const &);
  virtual void react(LoopEvent const &){};
  virtual void react(PowerOnEvent const &) {
    logger->log(LogLevel::DEBUG, "PowerOnEvent ignored");
  };
  virtual void react(BrewStartEvent const &) {
    logger->log(LogLevel::DEBUG, "BrewStartEvent ignored");
  };
  virtual void react(BrewStopEvent const &) {
    logger->log(LogLevel::DEBUG, "BrewStopEvent ignored");
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
  virtual void react(RinseStartEvent const &) {
    logger->log(LogLevel::DEBUG, "RinseStartEvent ignored");
  };
  virtual void react(RinseStopEvent const &) {
    logger->log(LogLevel::DEBUG, "RinseStopEvent ignored");
  };
  void react(TimeEvent const &e) { timestamp = e.timestamp; };

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

  // Timeout for the current state
  // It's up to each state to implement this themselves.
  static unsigned long timeout;

  // Current timestamp
  static unsigned long timestamp;

public:
  int getSetPoint() { return setpoint; };
  virtual PinStatus getSolenoid() { return LOW; }
  virtual uint8_t getPump() { return 0; }
  virtual long getStateUpdateInterval() { return STATE_UPDATE_INTERVAL; };
  virtual MachineMode getMode() { return MachineMode::UNKNOWN; };
};
