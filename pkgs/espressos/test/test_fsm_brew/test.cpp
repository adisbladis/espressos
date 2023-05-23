#include "../helpers.hpp"

#include <EventLoop.hpp>

TEST_SUITE("cycle") {
  TEST_CASE("turn on") {
    CHECK(MachineSignals::mode.get() == MachineMode::OFF);
    send_event(PowerOnEvent());
    CHECK(MachineSignals::mode.get() == MachineMode::IDLE);
  }

  TEST_CASE("start brewing") {
    send_event(BrewStartEvent());

    CHECK(MachineSignals::mode.get() == MachineMode::BREWING);
    CHECK(MachineSignals::pump.get().mode == PumpMode::POWER);
    CHECK(MachineSignals::pump.get().value == 0);
  }

  TEST_CASE("set pressure target") {
    BrewTargetEvent event;
    event.mode = BrewTargetEventMode::PRESSURE;
    event.value = 12345;
    send_event(event);

    CHECK(MachineSignals::pump.get().mode == PumpMode::PRESSURE);
    CHECK(MachineSignals::pump.get().value == 12345);
  }

  TEST_CASE("set power target") {
    BrewTargetEvent event;
    event.mode = BrewTargetEventMode::POWER;
    event.value = 54321;
    send_event(event);

    CHECK(MachineSignals::pump.get().mode == PumpMode::POWER);
    CHECK(MachineSignals::pump.get().value == 54321);
  }

  TEST_CASE("stop brewing") {
    send_event(BrewStopEvent());

    CHECK(MachineSignals::mode.get() == MachineMode::IDLE);
    CHECK(MachineSignals::pump.get().mode == PumpMode::POWER);
    CHECK(MachineSignals::pump.get().value == 0);
  }
}

TEST_MAIN()
