#pragma once

#include <Adafruit_MAX31865.h>
#include <Arduino.h>
#include <SimpleKalmanFilter.h>
#include <cstdint>
#include <dimmable_light.h>

#include "../../fsm//fsmlist.hpp"
#include "../../lib/effects.hpp"
#include "../../lib/map.hpp"
#include "../../lib/timers.hpp"

void setupArduinoPressureSensor(Timers &timers) {
  // TODO: Don't hardcode filter values and consider where this filter actually
  // belongs
  static SimpleKalmanFilter pressureKalmanFilter(0.6f, 0.6f, 0.1f);

  // Calculate the usable range of values from the ADC
  // A value of 0.8 means that we "lose" 20% of ADC range
  // as the sensor has a voltage range from 0.5v to 4.5v with a VCC of 5v.
  static std::uint16_t floor = (1024 - (1024 * PRESSURE_SENSOR_RANGE)) / 2;
  static std::uint16_t ceil = 1024 - floor;

  // We can calculate the floor nicely, but in reality these values have some
  // tolerances so it's best to allow for some reads outside of the measured
  // range.
  //
  // This sets the tolerance to the floor/ceiling +- 30%, it Works For Me™,
  // YMMV.
  static std::uint16_t minFloor = floor * 0.7;
  static std::uint16_t maxCeil = ceil * 1.3;

  timers.createInterval(PRESSURE_SENSOR_INTERVAL, []() {
    int value = analogRead(PRESSURE_SENSOR_PIN);

    // Clamp values outside of range to their respective max values
    if (value < floor && value >= minFloor) {
      value = floor;
    } else if (value > ceil && value <= maxCeil) {
      value = ceil;
    }

    if (value < floor || value > ceil) {
      send_event(PanicEvent());
      return;
    }

    value = pressureKalmanFilter.updateEstimate(value);

    PressureEvent e;
    e.pressure = (value - floor) * PRESSURE_SENSOR_MBAR / ceil;
    send_event(e);
  });
}

void setupArduinoTempSensor(Timers &timers) {
  static Adafruit_MAX31865 thermo(BOILER_MAX31865_SPI_PIN, BOILER_SPI_CLASS);
  thermo.begin();

  // Read temp and issue events
  timers.createInterval(PRESSURE_SENSOR_INTERVAL, []() {
    uint16_t rtd;
    bool cond = thermo.readRTDAsync(rtd);

    if (cond) {
      auto fault = thermo.readFault();

      if (fault) {
        thermo.clearFault();
        send_event(PanicEvent());
        return;
      }

      TempEvent tempEvent;
      tempEvent.temp =
          thermo.temperatureAsync(rtd, BOILER_RNOMINAL, BOILER_RREF) * 100;
      send_event(tempEvent);
    }
  });
}

void setupArduinoSolenoid(Effects &effects) {
  pinMode(BREW_SOLENOID_PIN, OUTPUT);
  effects.createEffect<PinStatus>(
      []() { return MachineState::current_state_ptr->getSolenoid(); },
      [](PinStatus pinStatus) { digitalWrite(BREW_SOLENOID_PIN, pinStatus); });
}

void setupArduinoPump(Effects &effects) {
  static DimmableLight pump(PUMP_DIMMER_OUT);

  DimmableLight::setSyncPin(PUMP_DIMMER_ZC);
  DimmableLight::begin();

  static PumpTarget lastPumpTarget = (PumpTarget){PumpMode::POWER, 0};
  static PIDController<int32_t, float, unsigned long> pressureProfilePID(
      0, 1, 7, 0, DIRECT);

  pressureProfilePID.Begin(AUTOMATIC, millis());

  static uint8_t pumpPower = 0;

  // React to state machine pump changes
  effects.createEffect<PumpTarget>(
      []() { return MachineState::current_state_ptr->getPump(); },
      [](PumpTarget pumpTarget) {
        switch (pumpTarget.mode) {
        case PumpMode::POWER:
          // Remap value into uint8 range used by dimmer and apply
          pumpPower = map<uint16_t>(pumpTarget.value, 0, 65535, 0, 255);
          break;

        case PumpMode::PRESSURE:
          pressureProfilePID.SetSetpoint(pumpTarget.value);
          break;

        default:
          // We don't know what we're doing, the only safe thing to do is to
          // stop.
          send_event(PanicEvent());
        }

        lastPumpTarget = pumpTarget;
      });

  // Run pump pressure profiling PID loop
  effects.createEffect<unsigned long>(
      []() { return MachineState::current_state_ptr->getTimestamp(); },
      [](unsigned long timestamp) {
        if (lastPumpTarget.mode != PumpMode::PRESSURE) {
          return;
        }

        int32_t output;
        if (pressureProfilePID.Compute(
                timestamp, MachineState::current_state_ptr->getPressure(),
                &output)) {
          pumpPower = output;
        }
      });

  // Write pump power
  effects.createEffect<uint8_t>(
      []() { return pumpPower; },
      [](uint8_t power) { pump.setBrightness(power); });
}

void setupHAL(Timers &timers, Effects &effects) {
  setupArduinoPressureSensor(timers);
  setupArduinoTempSensor(timers);
  setupArduinoSolenoid(effects);
  setupArduinoPump(effects);
}
