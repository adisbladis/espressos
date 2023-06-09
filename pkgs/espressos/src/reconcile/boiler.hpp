#pragma once

#include <PIDController.hpp>
#include <cstdint>

#include <EventLoop.hpp>
#include <Signal.hpp>
#include <Timers.hpp>

#include "../fsm/fsmlist.hpp"
#include "../fsm/signals.hpp"
#include "../proto/config.h"

void setupBoilerPID(Signal<bool> &outputState) {
  static constexpr int SampleTime = 100;

  static PIDController<int32_t, float> boilerPID(0, BOILER_PID_P, BOILER_PID_I,
                                                 BOILER_PID_D, DIRECT);

  boilerPID.SetSampleTime(SampleTime);
  boilerPID.SetMode(AUTOMATIC);

  ::MachineSignals::config.createEffect([](const Config &config) {
    auto pidConfig = config.get_boiler().get_PID();
    boilerPID.SetTunings(pidConfig.get_P(), pidConfig.get_I(),
                         pidConfig.get_D());
  });

  ::MachineSignals::setpoint.createEffect(
      [](const std::uint16_t &setpoint) { boilerPID.SetSetpoint(setpoint); });

  getEventLoop().createInterval(SampleTime, [&]() {
    std::int16_t temp = ::MachineSignals::temp.get();

    auto Output = boilerPID.Compute(temp);

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
      getEventLoop().setTimeout(Output, [&]() { outputState = false; });
    }
  });
}
