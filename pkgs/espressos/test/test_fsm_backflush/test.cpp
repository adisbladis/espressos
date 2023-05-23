#include "../helpers.hpp"

#include <EventLoop.hpp>
#include <cstdint>

TEST_SUITE("cycle") {
  TEST_CASE("turn on") {
    CHECK(MachineSignals::mode.get() == MachineMode::OFF);
    send_event(PowerOnEvent());
    CHECK(MachineSignals::mode.get() == MachineMode::IDLE);
  }

  TEST_CASE("start backflushing") {
    send_event(BackflushStartEvent());

    CHECK(MachineSignals::mode.get() == MachineMode::BACKFLUSHING);
    CHECK(MachineSignals::solenoid.get() == true);
    CHECK(MachineSignals::pump.get().mode == PumpMode::POWER);
    CHECK(MachineSignals::pump.get().value == 65535);
  }

  TEST_CASE("cycling") {
    long timestamp = 0;
    bool expectedSolenoid = true;
    uint16_t expectedValue = 65535;

    for (int i = 0; i < BACKFLUSH_ACTIVE_COUNT; i++) {
      CHECK(MachineSignals::solenoid.get() == expectedSolenoid);
      CHECK(MachineSignals::pump.get().mode == PumpMode::POWER);
      CHECK(MachineSignals::pump.get().value == expectedValue);

      timestamp +=
          expectedSolenoid ? BACKFLUSH_DUTY_CYCLE : BACKFLUSH_DUTY_CYCLE * 2;
      expectedValue = expectedValue == 0 ? 65535 : 0;
      expectedSolenoid = !expectedSolenoid;

      getEventLoop().loop(timestamp);
    }

    CHECK(MachineSignals::mode.get() == MachineMode::IDLE);
    CHECK(MachineSignals::solenoid.get() == false);
    CHECK(MachineSignals::pump.get().mode == PumpMode::POWER);
    CHECK(MachineSignals::pump.get().value == 0);
  }
}

TEST_MAIN()
