#include <cstdint>
#include <tinyfsm.hpp>

#include "backflush.hpp"
#include "events.hpp"
#include "fsmlist.hpp"
#include "machine.hpp"
#include "pump.hpp"
#include "rinse.hpp"

/* Forward declarations */
class Idle;
class Panic;
class Brewing;
class Pumping;
class Steaming;
class Backflushing;
class Rinsing;

/* Machine states */

// Panic (i.e. on hardware sensor errors) is exactly like the Off
// state but without a transition to other states
class Panic : public MachineState {
  void entry() override { setpoint = 0; }

public:
  MachineMode getMode() override { return MachineMode::PANIC; };
};

// The machine is idle with the boiler turned off
class Off : public MachineState {
  void entry() override { setpoint = 0; };

  void react(PowerOnEvent const &e) override {
    setpoint = e.setpoint;
    transit<Idle>();
  }

public:
  MachineMode getMode() override { return MachineMode::OFF; };
};

// Idle state - Meaning just sitting around with the boiler on
class Idle : public MachineState {
  MachineMode getMode() override { return MachineMode::IDLE; };

  void react(PowerOnEvent const &e) override {
    setpoint = e.setpoint;
    transit<Idle>();
  }

  void react(BrewStartEvent const &e) override { transit<Brewing>(); }

  void react(StartPumpEvent const &e) override { transit<Pumping>(); }

  void react(StartSteamEvent const &e) override {
    prevSetpoint = setpoint;
    setpoint = e.setpoint;
    timeout = timestamp + STEAM_TIMEOUT;
    transit<Steaming>();
  }

  void react(BackflushStartEvent const &e) override { transit<Backflushing>(); }

  void react(RinseStartEvent const &e) override { transit<Rinsing>(); }
};

// Brewing - We're actively brewing a cup
class Brewing : public MachineState {
  MachineMode getMode() override { return MachineMode::BREWING; };
  void react(BrewStopEvent const &e) override { transit<Idle>(); }

public:
  PinStatus getSolenoid() override { return HIGH; }
  PumpTarget getPump() override { return (PumpTarget){PRESSURE, 8500}; }
  long getStateUpdateInterval() override { return STATE_UPDATE_INTERVAL_BREW; }
};

class Backflushing : public MachineState {
  void react(BackflushStopEvent const &e) override { transit<Idle>(); }

public:
  MachineMode getMode() override { return MachineMode::BACKFLUSHING; };

  long getStateUpdateInterval() override { return STATE_UPDATE_INTERVAL_BREW; }

  PinStatus getSolenoid() override {
    if (BackflushState::current_state_ptr->active()) {
      return HIGH;
    } else {
      return LOW;
    }
  }

  PumpTarget getPump() override {
    if (BackflushState::current_state_ptr->active()) {
      return (PumpTarget){POWER, PumpMax};
    } else {
      return (PumpTarget){POWER, PumpMin};
    }
  }
};

class Rinsing : public MachineState {
  void react(RinseStopEvent const &e) override { transit<Idle>(); }

public:
  MachineMode getMode() override { return MachineMode::RINSING; };

  long getStateUpdateInterval() override { return STATE_UPDATE_INTERVAL_BREW; }

  PinStatus getSolenoid() override {
    if (RinseState::current_state_ptr->active()) {
      return HIGH;
    } else {
      return LOW;
    }
  }

  PumpTarget getPump() override {
    if (RinseState::current_state_ptr->active()) {
      return (PumpTarget){POWER, PumpMax};
    } else {
      return (PumpTarget){POWER, PumpMin};
    }
  }
};

class Pumping : public MachineState {
  void react(StopPumpEvent const &e) override { transit<Idle>(); }

public:
  PumpTarget getPump() override { return (PumpTarget){POWER, PumpMax}; };

  MachineMode getMode() override { return MachineMode::PUMPING; };
};

// Steaming - Boiler is set to the steam setpoint
class Steaming : public MachineState {
  void exit() override { setpoint = prevSetpoint; };

  void react(StopSteamEvent const &e) override { transit<Idle>(); }

  void react(LoopEvent const &e) override {
    if (e.timestamp >= timeout) {
      transit<Idle>();
    }
  }

public:
  MachineMode getMode() override { return MachineMode::STEAMING; };
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
void MachineState::react(PanicEvent const &e) { transit<Panic>(); }

std::uint16_t MachineState::setpoint = 0;
std::uint16_t MachineState::prevSetpoint = 0;
unsigned long MachineState::timeout = 0;
unsigned long MachineState::timestamp = 0;
std::uint16_t MachineState::pressure = 0;

/* Initial state */
FSM_INITIAL_STATE(MachineState, Off)
