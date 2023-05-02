#pragma once

#include <cstdint>
#include <tinyfsm.hpp>

#include "events.hpp"

class BrewState : public tinyfsm::Fsm<BrewState> {
  friend class tinyfsm::Fsm<BrewState>;

  virtual void entry(void){};
  virtual void exit(void){};

  virtual void react(LoopEvent const &){};

  virtual void react(tinyfsm::Event const &){};

  virtual void react(BrewStartEvent const &){};
  void react(BrewStopEvent const &);

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

public:
  uint32_t getShotStartTime() { return shotStartTime; };
  uint32_t getShotStopTime() { return shotStopTime; };
};
