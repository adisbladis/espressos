#pragma once

#include <PIDController.hpp>
#include <cstdint>

#include "../fsm/fsmlist.hpp"
#include "../fsm/signals.hpp"
#include "../lib/signal.hpp"
#include "../lib/timers.hpp"

void setupBoilerPID(Timers &timers, Signal<bool> &outputState) {
  static constexpr int SampleTime = 100;

  static PIDController<int32_t, float, unsigned long> boilerPID(
      0, BOILER_PID_P, BOILER_PID_I, BOILER_PID_D, DIRECT);

  boilerPID.SetSampleTime(SampleTime);
  boilerPID.SetMode(AUTOMATIC);

  ::MachineSignals::setpoint.createEffect(
      [](std::uint16_t setpoint) { boilerPID.SetSetpoint(setpoint); });

  timers.createInterval(SampleTime, [&]() {
    std::int16_t temp = ::MachineSignals::temp.get();
    std::int32_t Output;

    boilerPID.Compute(0, temp, &Output, true);

    // Output is off
    if (Output == 0) {
      outputState = false;
      return;
    }

    // Output is on
    outputState = true;

    // If the output is on for shorter than a sample window
    // schedule an async task to turn it off when appropriate.
    // It's useless to run the timer if it would run at the same time as the
    // interval as it would get recomputed immediately anyway.
    if (Output < SampleTime) {
      timers.setTimeout(Output, [&]() { outputState = false; });
    }
  });
}
