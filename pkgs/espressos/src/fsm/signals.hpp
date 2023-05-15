#pragma once

#include <cstdint>
#include <tinyfsm.hpp>

#include "../lib/signal.hpp"
#include "../proto/api.h"
#include "events.hpp"
#include "pump.hpp"

// Signals are sideeffectful singleton values shared by all state machines
// They are wrapped in a state machine to
class MachineSignals : public tinyfsm::Fsm<MachineSignals> {
  friend class tinyfsm::Fsm<MachineSignals>;

  void react(tinyfsm::Event const &){};

  void entry() {};
  void exit() {};

  void react(TimeEvent const &e) { timestamp = e.timestamp; };
  void react(PressureEvent const &e) { pressure = e.pressure; };
  void react(TempEvent const &e) { temp = e.temp; };

public:
  static Signal<bool> solenoid;

  // Boiler setpoint
  static Signal<std::uint16_t> setpoint;

  // Current FSM timestamp
  static Signal<unsigned long> timestamp;

  // Current FSM pressure
  static Signal<std::uint16_t> pressure;

  // Current FSM temp
  static Signal<std::uint16_t> temp;

  // How often to send state updates
  static Signal<unsigned long> stateUpdateInterval;

  // Pump target
  static Signal<PumpTarget> pump;

  // Overall mode
  static Signal<MachineMode> mode;

  // Shot timer
  static Signal<unsigned long> shotStartTime;
  static Signal<unsigned long> shotStopTime;
};
