#include "../helpers.hpp"

#include <EventLoop.hpp>

TEST_SUITE("cycle") {
  TEST_CASE("turn on") {
    CHECK(MachineSignals::mode.get() == MachineMode::OFF);
    send_event(PowerOnEvent());
    CHECK(MachineSignals::mode.get() == MachineMode::IDLE);
  }

  TEST_CASE("start pumping") {
    send_event(PumpStartEvent());

    CHECK(MachineSignals::mode.get() == MachineMode::PUMPING);
    CHECK(MachineSignals::pump.get().mode == PumpMode::POWER);
    CHECK(MachineSignals::pump.get().value == 65535);
  }

  TEST_CASE("stop pumping") {
    send_event(PumpStopEvent());

    CHECK(MachineSignals::mode.get() == MachineMode::IDLE);
    CHECK(MachineSignals::pump.get().mode == PumpMode::POWER);
    CHECK(MachineSignals::pump.get().value == 0);
  }
}

TEST_MAIN()
