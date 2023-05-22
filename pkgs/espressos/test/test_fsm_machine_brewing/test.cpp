#include "../helpers.hpp"

TEST_CASE("assert initial state") {
  CHECK(MachineSignals::mode.get() == MachineMode::OFF);
}

TEST_CASE("turn on") {
  send_event(PowerOnEvent());
  CHECK(MachineSignals::mode.get() == MachineMode::IDLE);
}

TEST_CASE("start brewing") {
  send_event(BrewStartEvent());
  CHECK(MachineSignals::mode.get() == MachineMode::BREWING);
}

TEST_CASE("assert signal values") {
  CHECK(MachineSignals::setpoint.get() == 9100);
  CHECK(MachineSignals::solenoid.get() == true);
  CHECK(MachineSignals::pump.get().mode == PumpMode::POWER);
  CHECK(MachineSignals::pump.get().value == 0);
}

TEST_CASE("stop brewing") {
  send_event(BrewStopEvent());
  CHECK(MachineSignals::mode.get() == MachineMode::IDLE);
}

TEST_MAIN()
