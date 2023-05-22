#include "../helpers.hpp"

TEST_CASE("assert initial state") {
  CHECK(MachineSignals::mode.get() == MachineMode::OFF);
}

TEST_CASE("turn on") {
  send_event(PowerOnEvent());
  CHECK(MachineSignals::mode.get() == MachineMode::IDLE);
}

TEST_CASE("assert signal values") {
  CHECK(MachineSignals::setpoint.get() == 9100);
  CHECK(MachineSignals::solenoid.get() == false);
  CHECK(MachineSignals::pump.get().mode == PumpMode::POWER);
  CHECK(MachineSignals::pump.get().value == 0);
}

TEST_CASE("assert can transition to other states") {
  send_event(BrewStartEvent());
  CHECK(MachineSignals::mode.get() == MachineMode::BREWING);
  send_event(BrewStopEvent());
  CHECK(MachineSignals::mode.get() == MachineMode::IDLE);

  send_event(PumpStartEvent());
  CHECK(MachineSignals::mode.get() == MachineMode::PUMPING);
  send_event(PumpStopEvent());
  CHECK(MachineSignals::mode.get() == MachineMode::IDLE);

  send_event(SteamStartEvent());
  CHECK(MachineSignals::mode.get() == MachineMode::STEAMING);
  send_event(SteamStopEvent());
  CHECK(MachineSignals::mode.get() == MachineMode::IDLE);

  send_event(BackflushStartEvent());
  CHECK(MachineSignals::mode.get() == MachineMode::BACKFLUSHING);
  send_event(BackflushStopEvent());
  CHECK(MachineSignals::mode.get() == MachineMode::IDLE);

  send_event(RinseStartEvent());
  CHECK(MachineSignals::mode.get() == MachineMode::RINSING);
  send_event(RinseStopEvent());
  CHECK(MachineSignals::mode.get() == MachineMode::IDLE);
}

TEST_MAIN()
