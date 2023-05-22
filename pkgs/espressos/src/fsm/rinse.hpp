#pragma once

#include <tinyfsm.hpp>

#include <Timers.hpp>

#include "events.hpp"

// This is for quickly rinsing the group head after a shot.
//
// Rinsing is done by:
// - Activating the pump & solenoid fully for RINSE_DUTY_CYCLE
// - Turn off pump & solenoid

#define RINSE_DUTY_CYCLE 1000

class RinseState : public tinyfsm::Fsm<RinseState> {
  friend class tinyfsm::Fsm<RinseState>;

  virtual void entry(void){};
  virtual void exit(void){};

  void react(RinseStoppingEvent const &e);

  virtual void react(RinseStartingEvent const &){};

  void react(tinyfsm::Event const &){};
};
