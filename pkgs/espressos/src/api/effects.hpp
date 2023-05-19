#pragma once

#include "../fsm/fsmlist.hpp"
#include "../fsm/signals.hpp"
#include "types.hpp"

// Set up effects that update the API state
void setupAPIEffects(StateUpdateMessage_t &stateUpdateMessage) {

  ::MachineSignals::setpoint.createEffect(
      [&](int setpoint) { stateUpdateMessage.set_setpoint(setpoint); });

  // Set boiler temp in API server
  ::MachineSignals::temp.createEffect(
      [&](std::int16_t temp) { stateUpdateMessage.set_boilerTemp(temp); });

  // Set pressure in API server
  ::MachineSignals::pressure.createEffect([&](std::uint16_t pressure) {
    stateUpdateMessage.set_pressure(pressure);
  });

  ::MachineSignals::shotStartTime.createEffect([&](uint32_t ts) {
    auto shotTimer = stateUpdateMessage.mutable_shot_timer();
    shotTimer.set_start(ts);
    stateUpdateMessage.set_shot_timer(shotTimer);
  });

  ::MachineSignals::shotStopTime.createEffect([&](uint32_t ts) {
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

  // Set panic reason
  ::MachineSignals::panicReason.createEffect([&](std::string panicReason) {
    auto fs = stateUpdateMessage.mutable_panic_reason();
    auto pr = panicReason.c_str();

    fs.set(pr,
           strlen(pr) > ERROR_MESSAGE_SIZE ? ERROR_MESSAGE_SIZE : strlen(pr));
    stateUpdateMessage.set_panic_reason(fs);
  });

  // Set current time
  ::MachineSignals::timestamp.createEffect(
      [&](Timestamp_t timestamp) { stateUpdateMessage.set_millis(timestamp); });

  // Set mode in API server
  ::MachineSignals::mode.createEffect(
      [&](MachineMode mode) { stateUpdateMessage.set_mode(mode); });
}
