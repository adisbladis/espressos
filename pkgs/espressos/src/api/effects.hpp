#pragma once

#include "../fsm/fsmlist.hpp"
#include "../fsm/signals.hpp"
#include "types.hpp"

// Set up effects that update the API state
void updateStateUpdateMessage(StateUpdateMessage_t &msg) {
  msg.set_setpoint(MachineSignals::setpoint.get_mut());
  msg.set_boilerTemp(MachineSignals::temp.get_mut());
  msg.set_pressure(MachineSignals::pressure.get_mut());

  // Shot timer
  {
    auto shotTimer = msg.mutable_shot_timer();
    shotTimer.set_start(MachineSignals::shotStartTime.get_mut());
    shotTimer.set_stop(MachineSignals::shotStopTime.get_mut());
    msg.set_shot_timer(shotTimer);
  }

  // Set current brew target
  {
    auto target = MachineSignals::pump.get_mut();
    auto brewTarget = msg.mutable_brew_target();
    brewTarget.set_value(target.value);

    switch (target.mode) {
    case PumpMode::POWER:
      brewTarget.set_mode(BrewTargetMode::POWER);
      break;
    case PumpMode::PRESSURE:
      brewTarget.set_mode(BrewTargetMode::PRESSURE);
      break;
    }

    msg.set_brew_target(brewTarget);
  }

  // Set panic reason
  {
    auto panicReason = MachineSignals::panicReason.get_mut();

    auto fs = msg.mutable_panic_reason();
    auto pr = panicReason.c_str();

    fs.set(pr,
           strlen(pr) > ERROR_MESSAGE_SIZE ? ERROR_MESSAGE_SIZE : strlen(pr));
    msg.set_panic_reason(fs);
  }

  // Set current time
  msg.set_millis(millis());
  msg.set_mode(MachineSignals::mode.get());
}
