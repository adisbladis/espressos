#pragma once

#include <PIDController.hpp>
#include <cstdint>

#include "../fsm/fsmlist.hpp"
#include "../fsm/signals.hpp"
#include "../lib/signal.hpp"
#include "../lib/timers.hpp"

void setupPumpPID(Timers &timers, Signal<uint8_t> &pumpPower) {
  static constexpr int SampleTime = 10;

  static PumpTarget lastPumpTarget = (PumpTarget){PumpMode::POWER, 0};
  static PIDController<int32_t, float, unsigned long> pressureProfilePID(
      0, 1, 3, 0, DIRECT);

  pressureProfilePID.SetMode(AUTOMATIC);
  pressureProfilePID.SetSampleTime(SampleTime);

  // React to state machine pump changes
  ::MachineSignals::pump.createEffect([&](PumpTarget pumpTarget) {
    switch (pumpTarget.mode) {
    case PumpMode::POWER:
      // Remap value into uint8 range used by dimmer and apply
      pumpPower = map<uint16_t>(pumpTarget.value, 0, 65535, 0, 255);
      break;

    case PumpMode::PRESSURE:
      pressureProfilePID.SetSetpoint(pumpTarget.value);
      break;

    default:
      // We don't know what we're doing, the only safe thing to do is to
      // stop.
      send_event(PanicEvent());
    }

    lastPumpTarget = pumpTarget;
  });

  // Run pump pressure profiling PID loop
  timers.createInterval(SampleTime, [&](unsigned long timestamp) {
    if (lastPumpTarget.mode != PumpMode::PRESSURE) {
      return;
    }

    int32_t output;
    if (pressureProfilePID.Compute(timestamp, ::MachineSignals::pressure.get(),
                                   &output, true)) {
      pumpPower = output;
    }
  });
}
