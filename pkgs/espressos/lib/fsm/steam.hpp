#pragma once

#include <cstdint>
#include <tinyfsm.hpp>

#include "events.hpp"
#include "signals.hpp"

class SteamState : public tinyfsm::Fsm<SteamState> {
  friend class tinyfsm::Fsm<SteamState>;

  virtual void entry(void){};
  virtual void exit(void){};

  virtual void react(tinyfsm::Event const &){};

  virtual void react(SteamStartingEvent const &){};
  void react(SteamStoppingEvent const &);
};
