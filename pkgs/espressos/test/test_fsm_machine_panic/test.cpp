#include "../helpers.hpp"

#include <events.hpp>
#include <fsmlist.hpp>

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

TEST_MAIN()
