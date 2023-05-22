#include <Time.hpp>

#include "signals.hpp"

Signal<bool> MachineSignals::solenoid(false);
Signal<std::uint16_t> MachineSignals::setpoint(0);
Signal<Timestamp_t> MachineSignals::timestamp(0);
Signal<std::uint16_t> MachineSignals::pressure(0);
Signal<std::uint16_t> MachineSignals::temp(0);
Signal<PumpTarget> MachineSignals::pump((PumpTarget){PumpMode::POWER, 0});
Signal<MachineMode> MachineSignals::mode(MachineMode::OFF);
Signal<Timestamp_t> MachineSignals::shotStartTime(0);
Signal<Timestamp_t> MachineSignals::shotStopTime(0);
Signal<Config> MachineSignals::config(([]() {
  Config config = Config();

  auto boiler = config.mutable_boiler();
  auto pid = boiler.mutable_PID();
  pid.set_P(BOILER_PID_P);
  pid.set_I(BOILER_PID_I);
  pid.set_D(BOILER_PID_D);
  boiler.set_PID(pid);
  config.set_boiler(boiler);

  config.set_setpoint(BOILER_SETPOINT);
  config.set_steamSetPoint(BOILER_STEAM_SETPOINT);

  return config;
})());

Signal<std::string> MachineSignals::panicReason("");

class MachineSignalsListening : public MachineSignals {};

FSM_INITIAL_STATE(MachineSignals, MachineSignalsListening)
