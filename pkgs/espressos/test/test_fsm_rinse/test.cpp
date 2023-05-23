#include "../helpers.hpp"

#include <EventLoop.hpp>

TEST_SUITE("cycle") {
  TEST_CASE("turn on") {
    CHECK(MachineSignals::mode.get() == MachineMode::OFF);
    send_event(PowerOnEvent());
    CHECK(MachineSignals::mode.get() == MachineMode::IDLE);
  }

  TEST_CASE("start rinsing") {
    send_event(RinseStartEvent());

    CHECK(MachineSignals::mode.get() == MachineMode::RINSING);
    CHECK(MachineSignals::solenoid.get() == true);
    CHECK(MachineSignals::pump.get().mode == PumpMode::POWER);
    CHECK(MachineSignals::pump.get().value == 65535);
  }

  TEST_CASE("Times out") {
    getEventLoop().loop(RINSE_DUTY_CYCLE);

    CHECK(MachineSignals::mode.get() == MachineMode::IDLE);
    CHECK(MachineSignals::solenoid.get() == false);
    CHECK(MachineSignals::pump.get().mode == PumpMode::POWER);
    CHECK(MachineSignals::pump.get().value == 0);
  }
}

TEST_MAIN()
