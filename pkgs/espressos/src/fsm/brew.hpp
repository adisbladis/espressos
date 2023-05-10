#pragma once

#include <cstdint>
#include <tinyfsm.hpp>

#include "events.hpp"

// Alias BrewTargetEvent as the data is the same, but the storage lifetime is
// different
using BrewStateTarget = BrewTargetEvent;
using BrewStateMode = BrewTargetEventMode;

class BrewState : public tinyfsm::Fsm<BrewState> {
  friend class tinyfsm::Fsm<BrewState>;

  virtual void entry(void){};
  virtual void exit(void){};

  virtual void react(LoopEvent const &){};

  virtual void react(tinyfsm::Event const &){};

  virtual void react(BrewStartEvent const &){};
  void react(BrewStopEvent const &);

  void react(BrewTargetEvent const &e) {
    target.mode = e.mode;
    target.value = e.value;
  };

  void react(TimeEvent const &e) { timestamp = e.timestamp; };

protected:
  // Shot start timestamp.
  // This is normally created by millis()
  static unsigned long shotStartTime;

  // Shot stop timestamp.
  // This is normally created by millis()
  // Note that during the shot this will be set to 0
  static unsigned long shotStopTime;

  // Current timestamp
  static unsigned long timestamp;

  // Current target
  static BrewStateTarget target;

public:
  BrewStateTarget getTarget() { return target; };
  uint32_t getShotStartTime() { return shotStartTime; };
  uint32_t getShotStopTime() { return shotStopTime; };
};
