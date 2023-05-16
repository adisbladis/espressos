#pragma once
#include <tinyfsm.hpp>

#include "events.hpp"

// Backflushing is done by:
// - Activating the pump & solenoid fully for BACKFLUSH_DUTY_CYCLE
// - Resting for BACKFLUSH_DUTY_CYCLE*2 seconds
// - Repeat BACKFLUSH_ACTIVE_COUNT times

#define BACKFLUSH_DUTY_CYCLE 5000
#define BACKFLUSH_ACTIVE_COUNT 5

class BackflushState : public tinyfsm::Fsm<BackflushState> {
  friend class tinyfsm::Fsm<BackflushState>;

  virtual void entry(void){}; // entry actions in some states
  virtual void exit(void){};  // exit actions in some states

  void react(BackflushStopEvent const &e);

  virtual void react(BackflushStartEvent const &){};

  virtual void react(BackflushActivateEvent const &){};
  virtual void react(BackflushDeactivateEvent const &){};

  void react(tinyfsm::Event const &){};

protected:
  static int activeCount;
};
