/********************************************************
 * PID Proportional on measurement Example
 * Setting the PID to use Proportional on measurement will
 * make the output move more smoothly when the setpoint
 * is changed.  In addition, it can eliminate overshoot
 * in certain processes like sous-vides.
 ********************************************************/

#include "PIDController.hpp"

// Define Variables we'll be connecting to
double Input, Output;
static constexpr double Setpoint = 100;

// Specify the links and initial tuning parameters
PIDController<double, double, unsigned long> myPID(
    Setpoint, 2, 5, 1, P_ON_M,
    PIDControllerDirection::DIRECT); // P_ON_M specifies that Proportional on
                                     // Measurement be used P_ON_E (Proportional
                                     // on Error) is the default behavior

void setup() {
  // initialize the variables we're linked to
  Input = analogRead(0);

  // turn the PID on
  myPID.Begin(PIDControllerMode::AUTOMATIC, millis());
}

void loop() {
  Input = analogRead(0);
  myPID.Compute(millis(), Input, &Output);
  analogWrite(3, Output);
}
