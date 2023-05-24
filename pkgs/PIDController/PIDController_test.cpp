#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

// Test cases adapted from
// https://github.com/br3ttb/Arduino-PID-Library/pull/107

#include "PIDController.hpp"
typedef PIDController<double, double> TestController;

TEST_CASE("direct") {
  // Define Variables we'll be connecting to
  constexpr double Setpoint = 100;
  double Input, Output;

  // Specify the links and initial tuning parameters
  TestController myPID(Setpoint, 2, 5, 1, DIRECT);

  // initialize the variables we're linked to
  Input = 50;

  myPID.SetMode(AUTOMATIC);

  for (int i = 0; i < 1000; i++) {
    Output = myPID.Compute(Input);
    Input = Input + (Output - Input) / 25.6;
  }

  CHECK(Setpoint == round(Input));
}

TEST_CASE("reverse") {
  // Define Variables we'll be connecting to
  constexpr double Setpoint = 100;
  double Input, Output;

  // Specify the links and initial tuning parameters
  TestController myPID(Setpoint, 2, 5, 1, REVERSE);

  // initialize the variables we're linked to
  Input = 50;

  myPID.SetMode(AUTOMATIC);

  for (int i = 0; i < 1000; i++) {
    Output = myPID.Compute(Input);
    Input = Input + (Input - Output) / 25.6;
  }

  CHECK(Setpoint == round(Input));
}

TEST_CASE("mode") {
  // Define Variables we'll be connecting to
  constexpr double Setpoint = 100;
  double Input, Output;

  // Specify the links and initial tuning parameters
  TestController myPID(Setpoint, 2, 5, 1, REVERSE);

  // initialize the variables we're linked to
  Input = 50;

  // turn the PID on
  myPID.SetMode(MANUAL);

  for (int i = 0; i < 10; i++) {
    Output = myPID.Compute(Input);
    Input = Input + (Input - Output) / 25.6;
  }

  CHECK(73 == round(Input));
}

TEST_CASE("getFunctions") {
  constexpr double Setpoint = 100;

  // Specify the links and initial tuning parameters
  TestController myPID(Setpoint, 2, 5, 1, REVERSE);

  // turn the PID on
  myPID.SetMode(AUTOMATIC);

  auto tunings = myPID.GetTunings();

  CHECK(2 == tunings.kp);
  CHECK(5 == tunings.ki);
  CHECK(1 == tunings.kd);
  CHECK(REVERSE == myPID.GetDirection());
  CHECK(AUTOMATIC == myPID.GetMode());
}
