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

// Machine states with sub state machines managing output signals
// share this template.
template <MachineMode mode, typename StartingEvent, typename StopEvent,
          typename StoppingEvent>
class MachineStateDecl : public MachineState {
  void react(StopEvent const &e) override { transit<Idle>(); }

  void entry() override {
    MachineSignals::mode = mode;
    send_event(StartingEvent());
  };

  void exit() override { send_event(StoppingEvent()); };
};

// States managed by their own state machines
class Brewing : public MachineStateDecl<MachineMode::BREWING, BrewStartingEvent,
                                        BrewStopEvent, BrewStoppingEvent> {};
class Backflushing
    : public MachineStateDecl<MachineMode::BACKFLUSHING, BackflushStartingEvent,
                              BackflushStopEvent, BackflushStoppingEvent> {};
class Rinsing
    : public MachineStateDecl<MachineMode::RINSING, RinseStartingEvent,
                              RinseStopEvent, RinseStoppingEvent> {};
class Pumping : public MachineStateDecl<MachineMode::PUMPING, PumpStartingEvent,
                                        PumpStopEvent, PumpStoppingEvent> {};
class Steaming
    : public MachineStateDecl<MachineMode::STEAMING, SteamStartingEvent,
                              SteamStopEvent, SteamStoppingEvent> {};

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

/* Initial state */
FSM_INITIAL_STATE(MachineState, Off)
