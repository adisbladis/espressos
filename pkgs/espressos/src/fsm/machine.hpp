#pragma once

#include <Arduino.h>
#include <cstdint>
#include <tinyfsm.hpp>

#include "../proto/api.h"
#include "backflush.hpp"
#include "events.hpp"
#include "fsmlist.hpp"
#include "pump.hpp"
#include "rinse.hpp"

/* Forward declarations */
class Idle;
class Panic;
class Brewing;
class Pumping;
class Steaming;

/* Events */
struct PowerOnEvent : tinyfsm::Event {
  std::uint16_t setpoint;
};
struct StartSteamEvent : tinyfsm::Event {
  std::uint16_t setpoint;
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
  virtual void react(PowerOnEvent const &){};
  virtual void react(BrewStartEvent const &){};
  virtual void react(BrewStopEvent const &){};
  virtual void react(StartPumpEvent const &) {}
  virtual void react(StopPumpEvent const &){};
  virtual void react(StartSteamEvent const &){};
  virtual void react(StopSteamEvent const &){};
  virtual void react(BackflushStartEvent const &){};
  virtual void react(BackflushStopEvent const &){};
  virtual void react(RinseStartEvent const &){};
  virtual void react(RinseStopEvent const &){};
  void react(TimeEvent const &e) { timestamp = e.timestamp; };
  void react(PressureEvent const &e) { pressure = e.pressure; };
  void react(TempEvent const &e) { temp = e.temp; };

  virtual void entry(void){}; // entry actions in some states
  virtual void exit(void){};  // exit actions in some states

protected:
  // Boiler setpoint
  static std::uint16_t setpoint;

  // Store the previous boiler setpoint when entering steam mode
  // so we can easily transition back into idle mode with the correct setpoint.
  static std::uint16_t prevSetpoint;

  // How often to emit state updates to clients
  static unsigned long stateUpdateInterval;

  // Timeout for the current state
  // It's up to each state to implement this themselves.
  static unsigned long timeout;

  // Current timestamp
  static unsigned long timestamp;

  // Current pressure
  static std::uint16_t pressure;

  // Current temp
  static std::int16_t temp;

public:
  std::uint16_t getSetPoint() { return setpoint; };
  unsigned long getTimestamp() { return timestamp; };
  std::uint16_t getPressure() { return pressure; };
  std::int16_t getTemp() { return temp; };
  virtual PinStatus getSolenoid() { return LOW; }
  virtual PumpTarget getPump() { return (PumpTarget){PumpMode::POWER, 0}; }
  virtual long getStateUpdateInterval() { return STATE_UPDATE_INTERVAL; };
  virtual MachineMode getMode() { return MachineMode::UNKNOWN; };
};
