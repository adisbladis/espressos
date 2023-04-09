#include <Adafruit_MAX31865.h>
#include <Arduino.h>
#include <PID_v1.h>

#include "boiler.hpp"
#include "cachedpin.hpp"

// Thermal readings
#define RREF 430.0 // Value of Rref resistor (PT100 == 430.0, PT1000 == 4300.0)
#define RNOMINAL                                                               \
  100.0 // "Nominal" 0-degrees resistance of the sensor (PT100 == 100.0, PT1000
        // == 1000.0)

BoilerPID::BoilerPID(double relayPin, double max31865SPIPin,
                     SPIClass *theSPI = &SPI)
    : pid(&Input, &Output, &Setpoint, Kp, Ki, Kd, REVERSE),
      thermo(max31865SPIPin, theSPI), outputPin(relayPin){};

void BoilerPID::setup() {
  outputPin.setup();

  thermo.begin(MAX31865_2WIRE);

  windowStartTime = millis();

  pid.SetOutputLimits(0, WindowSize); // PID output range
  pid.SetMode(AUTOMATIC);
}

void BoilerPID::SetSetPoint(double setPoint) { Setpoint = setPoint; }

double BoilerPID::GetSetPoint() { return Setpoint; }

struct TempReading BoilerPID::getTemp() {
  return temp;
}

void BoilerPID::SetTunings(double Kp, double Ki, double Kd) {
  pid.SetTunings(Kp, Ki, Kd);
}

bool BoilerPID::tick() {
  uint16_t rtd;
  bool cond = thermo.readRTDAsync(rtd);

  if (cond) {
    temp.fault = thermo.readFault();
    temp.temp = thermo.temperatureAsync(rtd, RNOMINAL, RREF);

    thermo.clearFault();

    // If a hardware error occured while checking temps disable the boiler
    if (temp.fault) {
      pid.SetMode(MANUAL);
      outputPin.digitalWrite(LOW);
      return cond;
    }

    // If a previous error occured we might have disabled the boiler
    if (pid.GetMode() != AUTOMATIC) {
      pid.SetMode(AUTOMATIC);
    }

    Input = temp.temp;
  }

  pid.Compute();

  auto now = millis();

  if (now - windowStartTime >= WindowSize) {
    windowStartTime += WindowSize;
  }

  if (Output < now - windowStartTime) {
    outputPin.digitalWrite(HIGH);
  } else {
    outputPin.digitalWrite(LOW);
  }

  return cond;
}
