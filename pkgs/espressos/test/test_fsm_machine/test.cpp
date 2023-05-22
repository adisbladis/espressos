#include "../helpers.hpp"

TEST_SUITE("backflushing") {

  TEST_CASE("assert initial state") {
    CHECK(MachineSignals::mode.get() == MachineMode::OFF);
  }

  TEST_CASE("turn on") {
    send_event(PowerOnEvent());
    CHECK(MachineSignals::mode.get() == MachineMode::IDLE);
  }

  TEST_CASE("start backflushing") {
    send_event(BackflushStartEvent());
    CHECK(MachineSignals::mode.get() == MachineMode::BACKFLUSHING);
  }

  TEST_CASE("assert signal values") {
    CHECK(MachineSignals::setpoint.get() == 9100);
    CHECK(MachineSignals::solenoid.get() == true);
    CHECK(MachineSignals::pump.get().mode == PumpMode::POWER);
    CHECK(MachineSignals::pump.get().value == 65535);
  }

  TEST_CASE("stop backflushing") {
    send_event(BackflushStopEvent());
    CHECK(MachineSignals::mode.get() == MachineMode::IDLE);
  }

  TEST_CASE("turn off") {
    send_event(PowerOffEvent());
    CHECK(MachineSignals::mode.get() == MachineMode::OFF);
  }
}

TEST_SUITE("brewing") {
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
}

TEST_SUITE("idle") {
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
}

TEST_SUITE("pumping") {
  TEST_CASE("assert initial state") {
    CHECK(MachineSignals::mode.get() == MachineMode::IDLE);
  }

  TEST_CASE("start pumping") {
    send_event(PumpStartEvent());
    CHECK(MachineSignals::mode.get() == MachineMode::PUMPING);
  }

  TEST_CASE("assert signal values") {
    CHECK(MachineSignals::setpoint.get() == 9100);
    CHECK(MachineSignals::solenoid.get() == false);
    CHECK(MachineSignals::pump.get().mode == PumpMode::POWER);
    CHECK(MachineSignals::pump.get().value == 65535);
  }

  TEST_CASE("stop pumping") {
    send_event(PumpStopEvent());
    CHECK(MachineSignals::mode.get() == MachineMode::IDLE);
  }
}

TEST_SUITE("rinsing") {
  TEST_CASE("assert initial state") {
    CHECK(MachineSignals::mode.get() == MachineMode::IDLE);
  }

  TEST_CASE("start backflushing") {
    send_event(RinseStartEvent());
    CHECK(MachineSignals::mode.get() == MachineMode::RINSING);
  }

  TEST_CASE("assert signal values") {
    CHECK(MachineSignals::setpoint.get() == 9100);
    CHECK(MachineSignals::solenoid.get() == true);
    CHECK(MachineSignals::pump.get().mode == PumpMode::POWER);
    CHECK(MachineSignals::pump.get().value == 65535);
  }

  TEST_CASE("stop backflushing") {
    send_event(RinseStopEvent());
    CHECK(MachineSignals::mode.get() == MachineMode::IDLE);
  }
}

TEST_SUITE("steaming") {
  TEST_CASE("assert initial state") {
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
}

TEST_SUITE("off") {
  TEST_CASE("turn on") {
    send_event(PowerOnEvent());
    CHECK(MachineSignals::mode.get() == MachineMode::IDLE);
  }

  TEST_CASE("turn off") {
    send_event(PowerOffEvent());
    CHECK(MachineSignals::mode.get() == MachineMode::OFF);
  }

  TEST_CASE("assert signal values") {
    CHECK(MachineSignals::setpoint.get() == 0);
    CHECK(MachineSignals::solenoid.get() == false);
    CHECK(MachineSignals::pump.get().mode == PumpMode::POWER);
    CHECK(MachineSignals::pump.get().value == 0);
  }
}

TEST_SUITE("panic") {
  TEST_CASE("assert initial state") {
    CHECK(MachineSignals::mode.get() != MachineMode::PANIC);
  }

  TEST_CASE("send panic") {
    PanicEvent event;
    event.reason = "FooBar";
    send_event(event);
  }

  TEST_CASE("assert mode signal") {
    CHECK(MachineSignals::mode.get() == MachineMode::PANIC);
  }

  TEST_CASE("assert panic reason signal") {
    CHECK(MachineSignals::panicReason.get() == "FooBar");
  }

  TEST_CASE("assert cannot transition to other states") {
    send_event(PowerOnEvent());
    CHECK(MachineSignals::mode.get() == MachineMode::PANIC);

    send_event(BrewStartEvent());
    CHECK(MachineSignals::mode.get() == MachineMode::PANIC);

    send_event(PumpStartEvent());
    CHECK(MachineSignals::mode.get() == MachineMode::PANIC);

    send_event(SteamStartEvent());
    CHECK(MachineSignals::mode.get() == MachineMode::PANIC);

    send_event(BackflushStartEvent());
    CHECK(MachineSignals::mode.get() == MachineMode::PANIC);

    send_event(RinseStartEvent());
    CHECK(MachineSignals::mode.get() == MachineMode::PANIC);
  }

  TEST_CASE("assert signal values") {
    CHECK(MachineSignals::setpoint.get() == 0);
    CHECK(MachineSignals::solenoid.get() == false);
    CHECK(MachineSignals::pump.get().mode == PumpMode::POWER);
    CHECK(MachineSignals::pump.get().value == 0);
  }
}

TEST_MAIN()
