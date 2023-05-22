#include "../helpers.hpp"

TEST_CASE("assert initial state") {
  CHECK(MachineSignals::mode.get() == MachineMode::OFF);
}

TEST_CASE("turn on") {
  send_event(PowerOnEvent());
  CHECK(MachineSignals::mode.get() == MachineMode::IDLE);
}

TEST_CASE("start steaming") {
  send_event(SteamStartEvent());
  CHECK(MachineSignals::mode.get() == MachineMode::STEAMING);
}

TEST_CASE("assert signal values") {
  CHECK(MachineSignals::setpoint.get() == 13500);
  CHECK(MachineSignals::solenoid.get() == false);
  CHECK(MachineSignals::pump.get().mode == PumpMode::POWER);
  CHECK(MachineSignals::pump.get().value == 0);
}

TEST_CASE("stop steaming") {
  send_event(SteamStopEvent());
  CHECK(MachineSignals::mode.get() == MachineMode::IDLE);
}

TEST_MAIN()
