#pragma once

#include <cstdint>
#include <tinyfsm.hpp>

#include "../logger.hpp"
#include "events.hpp"

class BrewState : public tinyfsm::Fsm<BrewState> {
  friend class tinyfsm::Fsm<BrewState>;

  virtual void entry(void){};
  virtual void exit(void){};

  virtual void react(LoopEvent const &){};

  virtual void react(tinyfsm::Event const &) {
    logger->log(LogLevel::DEBUG, "Got unhandled event");
  };

  virtual void react(BrewStartEvent const &) {
    logger->log(LogLevel::DEBUG, "Ignored BrewStartEvent");
  };
  void react(BrewStopEvent const &);

protected:
  // Shot start timestamp.
  // This is normally created by millis()
  static unsigned long shotStartTime;

  // Shot stop timestamp.
  // This is normally created by millis()
  // Note that during the shot this will be set to 0
  static unsigned long shotStopTime;

public:
  uint32_t getShotStartTime() { return shotStartTime; };
  uint32_t getShotStopTime() { return shotStopTime; };
};
