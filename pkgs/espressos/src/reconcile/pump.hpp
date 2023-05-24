#pragma once

#include <PIDController.hpp>
#include <cstdint>

#include <EventLoop.hpp>
#include <Signal.hpp>
#include <Timers.hpp>

#include "../fsm/fsmlist.hpp"
#include "../fsm/signals.hpp"

void setupPumpPID(Signal<uint8_t> &pumpPower) {
  static constexpr int SampleTime = 10;

  static PumpTarget lastPumpTarget = (PumpTarget){PumpMode::POWER, 0};
  static PIDController<int32_t, float> pressureProfilePID(0, 1, 3, 0, DIRECT);

  pressureProfilePID.SetMode(AUTOMATIC);
  pressureProfilePID.SetSampleTime(SampleTime);

  // React to state machine pump changes
  ::MachineSignals::pump.createEffect([&](const PumpTarget &pumpTarget) {
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
      PanicEvent event;
      event.reason = "Unknown pump mode";
      send_event(event);
    }

    lastPumpTarget = pumpTarget;
  });

  // Run pump pressure profiling PID loop
  getEventLoop().createInterval(SampleTime, [&]() {
    if (lastPumpTarget.mode != PumpMode::PRESSURE) {
      return;
    }

    pumpPower = pressureProfilePID.Compute(::MachineSignals::pressure.get());
  });
}
