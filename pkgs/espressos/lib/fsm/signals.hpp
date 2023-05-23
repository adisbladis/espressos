#pragma once

#include <cstdint>
#include <tinyfsm.hpp>

#include <Signal.hpp>
#include <Time.hpp>

#include "../proto/api.h"
#include "../proto/config.h"
#include "events.hpp"

enum struct PumpMode {
  // In POWER we're controlling the dimmer somewhat directly.
  // It has a range between 0 to 65535 and says how much power we should
  // deliver to the pump.
  POWER,

  // We're pressure profiling
  PRESSURE,
};

static constexpr uint16_t PumpMax = 65535;
static constexpr uint16_t PumpMin = 0;

// Describes the desired state of a pump
struct PumpTarget {
public:
  PumpMode mode;

  // The value carries a different meaning per mode:
  // - POWER
  //   A value between 0 and 65535 expressing the power.
  // - Pressure
  //   The target pressure in millibar.
  uint16_t value;

  bool operator==(PumpTarget const &rhs) const {
    return mode == rhs.mode && value == rhs.value;
  }

  bool operator!=(PumpTarget const &rhs) const { return !(*this == rhs); }
};

// Signals are sideeffectful singleton values shared by all state machines
// They are wrapped in a state machine to
class MachineSignals : public tinyfsm::Fsm<MachineSignals> {
  friend class tinyfsm::Fsm<MachineSignals>;

  void react(tinyfsm::Event const &){};

  void entry(){};
  void exit(){};

  void react(PressureEvent const &e) { pressure = e.pressure; };
  void react(TempEvent const &e) { temp = e.temp; };
  void react(SetpointSetEvent const &e) {
    // Ignore event if we're not in any active mode
    switch (mode.get()) {
    case MachineMode::PANIC:
    case MachineMode::OFF:
      return;
    }

    setpoint = e.setpoint;
  };

public:
  static Signal<bool> solenoid;

  // Boiler setpoint
  static Signal<std::uint16_t> setpoint;

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

  // If we're in panic a reason can be provided
  static Signal<std::string> panicReason;
};
