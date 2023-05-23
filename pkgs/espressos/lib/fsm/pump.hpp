#pragma once

#include <cstdint>
#include <tinyfsm.hpp>

#include "events.hpp"
#include "signals.hpp"

class PumpState : public tinyfsm::Fsm<PumpState> {
  friend class tinyfsm::Fsm<PumpState>;

  virtual void entry(void){};
  virtual void exit(void){};

  virtual void react(tinyfsm::Event const &){};

  virtual void react(PumpStartingEvent const &){};
  void react(PumpStoppingEvent const &);
};
