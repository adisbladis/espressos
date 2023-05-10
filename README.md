# EspressOS - The espresso operating system

### What's this?

This is the firmware and UI for my espresso machine.

### What can it do?

- Boiler PID temperature control

- Brewing mode
  Actuates the solenoid and the pump

- Steam mode
  Sets the machine to the steam setpoint, with an automatic rollback in 15 minutes

- Pump mode
  This just engages the pump, exactly like the pump button on the front of the machine.

- Backflushing mode
  This automates the backflushing process by actuating the pump/solenoid at an interval a number of times.

- Websocket API
  A websocket API using protobuf for communication with the machine.

- Web interface
  This is not yet embedded into the machine, but at some point I will get to this too.

- Pressure profiling

### On what platform does it run?

It runs on the rp2040 based Raspberry Pi Pico W board.

### Docs?

For now there are none.
I will get there eventually but for now this is in the exploration phase.
