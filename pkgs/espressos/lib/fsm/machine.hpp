#pragma once

#include <cstdint>
#include <tinyfsm.hpp>

#include "../proto/api.h"
#include "backflush.hpp"
#include "events.hpp"
#include "fsmlist.hpp"
#include "pump.hpp"
#include "rinse.hpp"
#include "signals.hpp"

class MachineState : public tinyfsm::Fsm<MachineState> {
  friend class tinyfsm::Fsm<MachineState>;

  void react(tinyfsm::Event const &){};

  void react(PanicEvent const &);
  void react(PowerOffEvent const &);
  virtual void react(PowerOnEvent const &){};
  virtual void react(BrewStartEvent const &){};
  virtual void react(BrewStopEvent const &){};
  virtual void react(PumpStartEvent const &) {}
  virtual void react(PumpStopEvent const &){};
  virtual void react(SteamStartEvent const &){};
  virtual void react(SteamStopEvent const &){};
  virtual void react(BackflushStartEvent const &){};
  virtual void react(BackflushStopEvent const &){};
  virtual void react(RinseStartEvent const &){};
  virtual void react(RinseStopEvent const &){};
  virtual void entry(void){}; // entry actions in some states
  virtual void exit(void){};  // exit actions in some states
};
