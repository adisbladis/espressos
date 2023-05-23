#include <cstdint>
#include <tinyfsm.hpp>

#include <EventLoop.hpp>
#include <Timers.hpp>

#include "backflush.hpp"
#include "brew.hpp"
#include "events.hpp"
#include "fsmlist.hpp"
#include "machine.hpp"
#include "pump.hpp"
#include "rinse.hpp"
#include "signals.hpp"

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
  void entry() override {
    ::MachineSignals::setpoint = 0;
    ::MachineSignals::solenoid = false;
    ::MachineSignals::mode = MachineMode::PANIC;
    ::MachineSignals::pump = (PumpTarget){PumpMode::POWER, PumpMin};
  }
};

// The machine is idle with the boiler turned off
class Off : public MachineState {
  void entry() override {
    ::MachineSignals::setpoint = 0;
    ::MachineSignals::solenoid = false;
    ::MachineSignals::mode = MachineMode::OFF;
    ::MachineSignals::pump = (PumpTarget){PumpMode::POWER, PumpMin};
  };

  void react(PowerOnEvent const &e) override { transit<Idle>(); }
};

// Idle state - Meaning just sitting around with the boiler on
class Idle : public MachineState {
  void entry() override {
    ::MachineSignals::solenoid = false;
    ::MachineSignals::mode = MachineMode::IDLE;
    ::MachineSignals::setpoint = ::MachineSignals::config.get().get_setpoint();
  };
  void exit() override {}

  void react(PowerOnEvent const &e) override { transit<Idle>(); }

  void react(BrewStartEvent const &e) override { transit<Brewing>(); }

  void react(PumpStartEvent const &e) override { transit<Pumping>(); }

  void react(SteamStartEvent const &e) override { transit<Steaming>(); }

  void react(BackflushStartEvent const &e) override { transit<Backflushing>(); }

  void react(RinseStartEvent const &e) override { transit<Rinsing>(); }
};

// Brewing - We're actively brewing a cup
class Brewing : public MachineState {
  void react(BrewStopEvent const &e) override { transit<Idle>(); }

  void entry() override {
    ::MachineSignals::mode = MachineMode::BREWING;
    send_event(BrewStartingEvent());
  };

  void exit() override { send_event(BrewStoppingEvent()); };
};

class Backflushing : public MachineState {
  void react(BackflushStopEvent const &) override { transit<Idle>(); }

  void entry() override {
    ::MachineSignals::mode = MachineMode::BACKFLUSHING;
    send_event(BackflushStartingEvent());
  };

  void exit() override { send_event(BackflushStoppingEvent()); };
};

class Rinsing : public MachineState {
  void react(RinseStopEvent const &e) override { transit<Idle>(); }

  void entry() override {
    ::MachineSignals::mode = MachineMode::RINSING;
    send_event(RinseStartingEvent());
  };

  void exit() override { send_event(RinseStoppingEvent()); };
};

class Pumping : public MachineState {
  void react(PumpStopEvent const &e) override { transit<Idle>(); }

  void entry() override {
    ::MachineSignals::mode = MachineMode::PUMPING;
    send_event(PumpStartingEvent());
  };

  void exit() override { send_event(PumpStoppingEvent()); };
};

// Steaming - Boiler is set to the steam setpoint
class Steaming : public MachineState {
  void entry() override {
    ::MachineSignals::mode = MachineMode::STEAMING;
    ::MachineSignals::setpoint =
        ::MachineSignals::config.get().get_steamSetPoint();

    timeout = getEventLoop().setTimeout(STEAM_TIMEOUT,
                                        []() { send_event(SteamStopEvent()); });

    send_event(SteamStartingEvent());
  };

  void exit() override {
    timeout->cancel();
    send_event(SteamStoppingEvent());
  };

  void react(SteamStopEvent const &e) override { transit<Idle>(); }

protected:
  static Timeout_t timeout;
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
  ::MachineSignals::panicReason = e.reason;
  transit<Panic>();
}

Timeout_t Steaming::timeout = getEventLoop().setTimeout(0, []() {});

/* Initial state */
FSM_INITIAL_STATE(MachineState, Off)
