#pragma once

#include <tinyfsm.hpp>

#include "../logger.hpp"
#include "events.hpp"

// This is for quickly rinsing the group head after a shot.
//
// Rinsing is done by:
// - Activating the pump & solenoid fully for RINSE_DUTY_CYCLE
// - Turn off pump & solenoid

#define RINSE_DUTY_CYCLE 1000

struct RinseStartEvent : tinyfsm::Event {
  unsigned long timestamp;
};
struct RinseStopEvent : tinyfsm::Event {};

class RinseState : public tinyfsm::Fsm<RinseState> {
  friend class tinyfsm::Fsm<RinseState>;

  void entry(void){};
  void exit(void){};

  virtual void react(LoopEvent const &){};

  void react(RinseStopEvent const &e);

  virtual void react(RinseStartEvent const &) {
    logger->log(LogLevel::DEBUG, "RinseStartEvent ignored");
  };

  void react(tinyfsm::Event const &) {
    logger->log(LogLevel::DEBUG, "Got unhandled event");
  };

protected:
  static unsigned long timeout;

public:
  virtual bool active() { return false; }
};
