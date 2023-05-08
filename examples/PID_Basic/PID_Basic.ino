/********************************************************
 * PID Basic Example
 * Reading analog input 0 to control analog PWM output 3
 ********************************************************/

#include "PIDController.hpp"

#define PIN_INPUT 0
#define PIN_OUTPUT 3

// Define Variables we'll be connecting to
double Input, Output;
static constexpr double Setpoint = 100;

// Specify the links and initial tuning parameters
double Kp = 2, Ki = 5, Kd = 1;
PIDController<double, double, unsigned long>
    myPID(Setpoint, Kp, Ki, Kd, PIDControllerDirection::DIRECT);

void setup() {
  // initialize the variables we're linked to
  Input = analogRead(PIN_INPUT);

  // turn the PID on
  myPID.Begin(PIDControllerMode::AUTOMATIC, millis());
}

void loop() {
  Input = analogRead(PIN_INPUT);
  myPID.Compute(millis(), Input, &Output);
  analogWrite(PIN_OUTPUT, Output);
}
