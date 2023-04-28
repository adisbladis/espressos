#pragma once

#include <tinyfsm.hpp>

#include "../logger.hpp"
#include "events.hpp"

class BrewState : public tinyfsm::Fsm<BrewState> {
  friend class tinyfsm::Fsm<BrewState>;

  virtual void entry(void){}; // entry actions in some states
  virtual void exit(void){};  // exit actions in some states

  virtual void react(LoopEvent const &){};

  virtual void react(tinyfsm::Event const &) {
    logger->log(LogLevel::DEBUG, "Got unhandled event");
  };

  virtual void react(BrewStartEvent const &) {
    logger->log(LogLevel::DEBUG, "Ignored BrewStartEvent");
  };
};
