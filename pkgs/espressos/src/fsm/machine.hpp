#pragma once

#include <cstdint>
#include <tinyfsm.hpp>

#include "../proto/api.h"
#include "backflush.hpp"
#include "events.hpp"
#include "fsmlist.hpp"
#include "pump.hpp"
#include "rinse.hpp"
#include "signals.hpp"

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
  virtual void entry(void){}; // entry actions in some states
  virtual void exit(void){};  // exit actions in some states

protected:
  // Store the previous boiler setpoint when entering steam mode
  // so we can easily transition back into idle mode with the correct setpoint.
  static std::uint16_t prevSetpoint;

  // Timeout for the current state
  // It's up to each state to implement this themselves.
  static unsigned long timeout;
};
