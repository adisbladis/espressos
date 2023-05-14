#pragma once

#include "../fsm/fsmlist.hpp"
#include "../fsm/signals.hpp"
#include "types.hpp"

// Set up effects that update the API state
void setupAPIEffects(StateUpdateMessage_t &stateUpdateMessage) {

  ::MachineSignals::setpoint.createEffect(
      [&](int setpoint) { stateUpdateMessage.set_setpoint(setpoint); });

  // Set boiler temp in API server
  ::MachineSignals::temp.createEffect([&](std::int16_t temp) {
    auto boilerTempMsg = stateUpdateMessage.mutable_boilerTemp();
    boilerTempMsg.set_value(temp);
    stateUpdateMessage.set_boilerTemp(boilerTempMsg);
  });

  // Set pressure in API server
  ::MachineSignals::pressure.createEffect([&](std::uint16_t pressure) {
    auto pressureMsg = stateUpdateMessage.mutable_pressure();
    pressureMsg.set_value(pressure);
    stateUpdateMessage.set_pressure(pressureMsg);
  });

  ::MachineSignals::shotStartTime.createEffect(
      [&](uint32_t ts) {
        auto shotTimer = stateUpdateMessage.mutable_shot_timer();
        shotTimer.set_start(ts);
        stateUpdateMessage.set_shot_timer(shotTimer);
      });

  ::MachineSignals::shotStopTime.createEffect(
      [&](uint32_t ts) {
        auto shotTimer = stateUpdateMessage.mutable_shot_timer();
        shotTimer.set_stop(ts);
        stateUpdateMessage.set_shot_timer(shotTimer);
      });

  // Set current brew target
  ::MachineSignals::pump.createEffect([&](PumpTarget target) {
    auto brewTarget = stateUpdateMessage.mutable_brew_target();
    brewTarget.set_value(target.value);

    switch (target.mode) {
    case PumpMode::POWER:
      brewTarget.set_mode(BrewTargetMode::POWER);
      break;
    case PumpMode::PRESSURE:
      brewTarget.set_mode(BrewTargetMode::PRESSURE);
      break;
    }

    stateUpdateMessage.set_brew_target(brewTarget);
  });

  // Set current time
  ::MachineSignals::timestamp.createEffect([&](unsigned long timestamp) {
    stateUpdateMessage.set_millis(timestamp);
  });

  // Set mode in API server
  ::MachineSignals::mode.createEffect(
      [&](MachineMode mode) { stateUpdateMessage.set_mode(mode); });
}
