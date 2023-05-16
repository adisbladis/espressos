#pragma once

#include <cstdint>
#include <tinyfsm.hpp>

#include "events.hpp"
#include "signals.hpp"

// Alias BrewTargetEvent as the data is the same, but the storage lifetime is
// different
using BrewStateTarget = BrewTargetEvent;
using BrewStateMode = BrewTargetEventMode;

class BrewState : public tinyfsm::Fsm<BrewState> {
  friend class tinyfsm::Fsm<BrewState>;

  virtual void entry(void){};
  virtual void exit(void){};

  virtual void react(tinyfsm::Event const &){};

  virtual void react(BrewStartEvent const &){};
  void react(BrewStopEvent const &);

  virtual void react(BrewTargetEvent const &){};
};
