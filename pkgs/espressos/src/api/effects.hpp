#pragma once

#include "../fsm/fsmlist.hpp"
#include "../lib/effects.hpp"
#include "types.hpp"

void setupAPIEffects(Effects &effects,
                     StateUpdateMessage_t &stateUpdateMessage) {
  effects.createEffect<int>(
      []() { return MachineState::current_state_ptr->getSetPoint(); },
      [&](int setpoint) { stateUpdateMessage.set_setpoint(setpoint); });

  // Set boiler temp in API server
  effects.createEffect<std::int16_t>(
      []() { return MachineState::current_state_ptr->getTemp(); },
      [&](std::int16_t temp) {
        auto boilerTempMsg = stateUpdateMessage.mutable_boilerTemp();
        boilerTempMsg.set_value(temp);
        stateUpdateMessage.set_boilerTemp(boilerTempMsg);
      },
      false); // Update cadence too high to use as update trigger

  // Set pressure in API server
  effects.createEffect<std::uint16_t>(
      []() { return MachineState::current_state_ptr->getPressure(); },
      [&](std::uint16_t pressure) {
        auto pressureMsg = stateUpdateMessage.mutable_pressure();
        pressureMsg.set_value(pressure);
        stateUpdateMessage.set_pressure(pressureMsg);
      },
      false); // Update cadence too high to use as update trigger

  effects.createEffect<uint32_t>(
      []() { return BrewState::current_state_ptr->getShotStartTime(); },
      [&](uint32_t ts) {
        auto shotTimer = stateUpdateMessage.mutable_shot_timer();
        shotTimer.set_start(ts);
        stateUpdateMessage.set_shot_timer(shotTimer);
      });

  effects.createEffect<uint32_t>(
      []() { return BrewState::current_state_ptr->getShotStopTime(); },
      [&](uint32_t ts) {
        auto shotTimer = stateUpdateMessage.mutable_shot_timer();
        shotTimer.set_stop(ts);
        stateUpdateMessage.set_shot_timer(shotTimer);
      });

  // Set current brew target
  effects.createEffect<BrewStateTarget>(
      []() { return BrewState::current_state_ptr->getTarget(); },
      [&](BrewStateTarget target) {
        auto brewTarget = stateUpdateMessage.mutable_brew_target();
        brewTarget.set_value(target.value);

        switch (target.mode) {
        case BrewStateMode::POWER:
          brewTarget.set_mode(BrewTargetMode::POWER);
          break;
        case BrewStateMode::PRESSURE:
          brewTarget.set_mode(BrewTargetMode::PRESSURE);
          break;
        }

        stateUpdateMessage.set_brew_target(brewTarget);
      },
      false); // Update cadence too high to use as a message trigger

  // Set current time
  effects.createEffect<unsigned long>(
      []() { return MachineState::current_state_ptr->getTimestamp(); },
      [&](unsigned long timestamp) {
        stateUpdateMessage.set_millis(timestamp);
      },
      false); // Update cadence too high to use as a message trigger

  // Set mode in API server
  effects.createEffect<MachineMode>(
      []() { return MachineState::current_state_ptr->getMode(); },
      [&](MachineMode mode) { stateUpdateMessage.set_mode(mode); });
}
