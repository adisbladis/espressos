#pragma once

#include <cstdint>
#include <tinyfsm.hpp>

#include "../hal/time.hpp"
#include "../lib/signal.hpp"
#include "../proto/api.h"
#include "../proto/config.h"
#include "events.hpp"
#include "pump.hpp"

// Signals are sideeffectful singleton values shared by all state machines
// They are wrapped in a state machine to
class MachineSignals : public tinyfsm::Fsm<MachineSignals> {
  friend class tinyfsm::Fsm<MachineSignals>;

  void react(tinyfsm::Event const &){};

  void entry(){};
  void exit(){};

  void react(PressureEvent const &e) { pressure = e.pressure; };
  void react(TempEvent const &e) { temp = e.temp; };

public:
  static Signal<bool> solenoid;

  // Boiler setpoint
  static Signal<std::uint16_t> setpoint;

  // Current FSM timestamp
  static Signal<Timestamp_t> timestamp;

  // Current FSM pressure
  static Signal<std::uint16_t> pressure;

  // Current FSM temp
  static Signal<std::uint16_t> temp;

  // Pump target
  static Signal<PumpTarget> pump;

  // Overall mode
  static Signal<MachineMode> mode;

  // Shot timer
  static Signal<Timestamp_t> shotStartTime;
  static Signal<Timestamp_t> shotStopTime;

  // Current configuration
  static Signal<Config> config;
};
