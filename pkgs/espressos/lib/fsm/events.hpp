#pragma once

#include <cstdint>
#include <iostream>
#include <tinyfsm.hpp>

#include <Time.hpp>

#include "../proto/config.h"

// The general flow around an action is as follows (using backflushing as an
// example):
// - User initiates an event (BackflushStartEvent).
// - MachineState is responsible for handling this action.
// - When transiting into Backflushing a BackflushStartingEvent is sent.
// - The backflush state machine listens to BackflushStartingEvent to transit
//   into whatever it's relevant state is.
// - When MachineState wants exits Backflushing it sends a
//   BackflushStoppingEvent, telling the backflush state machine to
//   go back to it's idle state, MachineState can then transit state.

// When the machine pressure changes a pressure event is emitted

// Pressure reading
struct PressureEvent : tinyfsm::Event {
  uint16_t pressure;

  PressureEvent(){};
  PressureEvent(uint16_t pressure) : pressure(pressure){};
};

// Temp reading
struct TempEvent : tinyfsm::Event {
  int16_t temp;
};

// Unrecoverable error
struct PanicEvent : tinyfsm::Event {
  std::string reason;
  PanicEvent(){};
  PanicEvent(std::string reason) : reason(reason){};
};

// Config
struct ConfigSetEvent : tinyfsm::Event {
  Config config;

  ConfigSetEvent(const Config &config) : config(config){};
};

// Set ephemeral setpoint
struct SetpointSetEvent : tinyfsm::Event {
  uint16_t setpoint;

  SetpointSetEvent(uint16_t setpoint) : setpoint(setpoint){};
};

// Power on/off
struct PowerOnEvent : tinyfsm::Event {};
struct PoweringOnEvent : tinyfsm::Event {};
struct PowerOffEvent : tinyfsm::Event {};
struct PoweringOffEvent : tinyfsm::Event {};

// Backflushing
struct BackflushStartEvent : tinyfsm::Event {};
struct BackflushStartingEvent : tinyfsm::Event {};
struct BackflushStopEvent : tinyfsm::Event {};
struct BackflushStoppingEvent : tinyfsm::Event {};

// Rinsing
struct RinseStartEvent : tinyfsm::Event {};
struct RinseStartingEvent : tinyfsm::Event {};
struct RinseStopEvent : tinyfsm::Event {};
struct RinseStoppingEvent : tinyfsm::Event {};

// Pumping
struct PumpStartEvent : tinyfsm::Event {};
struct PumpStartingEvent : tinyfsm::Event {};
struct PumpStopEvent : tinyfsm::Event {};
struct PumpStoppingEvent : tinyfsm::Event {};

// Steaming
struct SteamStartingEvent : tinyfsm::Event {};
struct SteamStartEvent : tinyfsm::Event {};
struct SteamStoppingEvent : tinyfsm::Event {};
struct SteamStopEvent : tinyfsm::Event {};

// Brewing
struct BrewStartEvent : tinyfsm::Event {};
struct BrewStartingEvent : tinyfsm::Event {};
struct BrewStopEvent : tinyfsm::Event {};
struct BrewStoppingEvent : tinyfsm::Event {};

enum struct BrewTargetEventMode { POWER, PRESSURE };
struct BrewTargetEvent : tinyfsm::Event {
  BrewTargetEventMode mode;
  uint16_t value;

  bool operator==(BrewTargetEvent const &rhs) const {
    return mode == rhs.mode && value == rhs.value;
  }

  bool operator!=(BrewTargetEvent const &rhs) const { return !(*this == rhs); }
};
