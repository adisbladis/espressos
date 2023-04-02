#include <Adafruit_MAX31865.h>
#include <Arduino.h>
#include <PID_v1.h>

#include "boiler.h"

// Thermal readings
#define RREF 430.0 // Value of Rref resistor (PT100 == 430.0, PT1000 == 4300.0)
#define RNOMINAL                                                               \
  100.0 // "Nominal" 0-degrees resistance of the sensor (PT100 == 100.0, PT1000
        // == 1000.0)

BoilerPID::BoilerPID(double relayPin, double max31865SPIPin,
                     SPIClass *theSPI = &SPI)
    : pid(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT),
      thermo(max31865SPIPin, theSPI) {
  outputPin = relayPin;
};

void BoilerPID::setup() {
  pinMode(outputPin, OUTPUT);

  thermo.begin(MAX31865_2WIRE);

  windowStartTime = millis();
  pid.SetOutputLimits(0, WindowSize); // PID output range
  pid.SetMode(AUTOMATIC);
}

void BoilerPID::SetSetPoint(double setPoint) { Setpoint = setPoint; }

double BoilerPID::GetSetPoint() { return Setpoint; }

struct TempReading BoilerPID::getTemp() {
  struct TempReading result;

  uint16_t rtd = thermo.readRTD();
  result.temp = thermo.temperature(RNOMINAL, RREF);

  // Check if any errors occured
  result.fault = thermo.readFault();
  thermo.clearFault();

  return result;
}

void BoilerPID::SetTunings(double Kp, double Ki, double Kd) {
  pid.SetTunings(Kp, Ki, Kd);
}

TempReading BoilerPID::tick() {
  unsigned long start = millis();

  struct TempReading temp = getTemp();

  // If a hardware error occured while checking temps disable the boiler
  if (temp.fault) {
    pid.SetMode(MANUAL);
    digitalWrite(outputPin, LOW);
    return temp;
  }

  // If a previous error occured we might have disabled the boiler
  if (pid.GetMode() != AUTOMATIC) {
    pid.SetMode(AUTOMATIC);
  }

  Input = temp.temp;
  pid.Compute();

  if (millis() - windowStartTime >= WindowSize) {
    windowStartTime += WindowSize;
  }
  if (Output > 0 && Output < millis() - windowStartTime) {
    digitalWrite(outputPin, HIGH);
  } else {
    digitalWrite(outputPin, LOW);
  }

  Serial.println(millis() - start);

  return temp;
}
