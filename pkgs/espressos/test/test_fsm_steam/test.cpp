#include "../helpers.hpp"

#include <EventLoop.hpp>

TEST_CASE("turn on") {
  CHECK(MachineSignals::mode.get() == MachineMode::OFF);
  send_event(PowerOnEvent());
  CHECK(MachineSignals::mode.get() == MachineMode::IDLE);
}

TEST_SUITE("cycle") {
  TEST_CASE("start steaming") {
    send_event(SteamStartEvent());

    CHECK(MachineSignals::mode.get() == MachineMode::STEAMING);
    CHECK(MachineSignals::setpoint.get() == 13500);
  }

  TEST_CASE("stop steaming") {
    send_event(SteamStopEvent());

    CHECK(MachineSignals::mode.get() == MachineMode::IDLE);
    CHECK(MachineSignals::setpoint.get() == 9100);
  }
}

TEST_SUITE("timeout") {
  TEST_CASE("start steaming") {
    send_event(SteamStartEvent());

    CHECK(MachineSignals::mode.get() == MachineMode::STEAMING);
    CHECK(MachineSignals::setpoint.get() == 13500);
  }

  TEST_CASE("times out") {
    getEventLoop().loop(STEAM_TIMEOUT);

    CHECK(MachineSignals::mode.get() == MachineMode::IDLE);
    CHECK(MachineSignals::setpoint.get() == 9100);
  }
}

TEST_MAIN()
