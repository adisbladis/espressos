#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

// Test cases adapted from
// https://github.com/br3ttb/Arduino-PID-Library/pull/107

#include "PIDController.hpp"
typedef PIDController<double, double, unsigned long> TestController;

TEST_CASE("direct") {
  // Define Variables we'll be connecting to
  double Setpoint, Input, Output;

  // Specify the links and initial tuning parameters
  TestController myPID(&Input, &Output, &Setpoint, 2, 5, 1, DIRECT);

  // initialize the variables we're linked to
  Input = 50;
  Setpoint = 100;

  unsigned long ms = 0;

  // turn the PID on
  myPID.Begin(AUTOMATIC, ms);
  for (int i = 0; i < 1000; i++) {
    ms += 200;

    if (myPID.Compute(ms)) {
      Input = Input + (Output - Input) / 25.6;
    }
  }

  CHECK(Setpoint == round(Input));
}

TEST_CASE("reverse") {
  // Define Variables we'll be connecting to
  double Setpoint, Input, Output;

  // Specify the links and initial tuning parameters
  TestController myPID(&Input, &Output, &Setpoint, 2, 5, 1, REVERSE);

  // initialize the variables we're linked to
  Input = 50;
  Setpoint = 100;

  unsigned long ms = 0;

  // turn the PID on
  myPID.Begin(AUTOMATIC, ms);
  for (int i = 0; i < 1000; i++) {
    ms += 200;

    if (myPID.Compute(ms)) {
      Input = Input + (Input - Output) / 25.6;
    }
  }

  CHECK(Setpoint == round(Input));
}

TEST_CASE("mode") {
  // Define Variables we'll be connecting to
  double Setpoint, Input, Output;

  // Specify the links and initial tuning parameters
  TestController myPID(&Input, &Output, &Setpoint, 2, 5, 1, REVERSE);

  // initialize the variables we're linked to
  Input = 50;
  Setpoint = 100;

  unsigned long ms = 0;

  // turn the PID on
  myPID.Begin(MANUAL, ms);
  for (int i = 0; i < 1000; i++) {
    ms += 200;

    if (myPID.Compute(ms)) {
      Input = Input + (Input - Output) / 25.6;
    }
  }

  CHECK(50 == round(Input));
}

TEST_CASE("getFunctions") {

  // Define Variables we'll be connecting to
  double Setpoint, Input, Output;

  // Specify the links and initial tuning parameters
  TestController myPID(&Input, &Output, &Setpoint, 2, 5, 1, REVERSE);

  // initialize the variables we're linked to
  Input = 50;
  Setpoint = 100;

  // turn the PID on
  myPID.SetMode(AUTOMATIC);

  // std::cout << "Input = " << Input << "; Output = " << Output << std::endl;
  CHECK(2 == myPID.GetKp());
  CHECK(5 == myPID.GetKi());
  CHECK(1 == myPID.GetKd());
  CHECK(REVERSE == myPID.GetDirection());
  CHECK(AUTOMATIC == myPID.GetMode());
}

TEST_CASE("sampleTimeWorks") {

  // Define Variables we'll be connecting to
  double Setpoint, Input, Output;

  // Specify the links and initial tuning parameters
  TestController myPID(&Input, &Output, &Setpoint, 2, 5, 1, REVERSE);

  // initialize the variables we're linked to
  Input = 50;
  Setpoint = 100;

  unsigned long ms = 0;

  bool flag = false;

  // turn the PID on
  myPID.SetMode(AUTOMATIC);
  for (int i = 0; i < 1000; i++) {
    ms += 200;

    if (myPID.Compute(ms)) {
      flag = true;
      Input = Input + (Input - Output) / 25.6;
    }
  }

  CHECK(flag == true);
}

TEST_CASE("sampleTimeNotWorks") {

  // Define Variables we'll be connecting to
  double Setpoint, Input, Output;

  // Specify the links and initial tuning parameters
  TestController myPID(&Input, &Output, &Setpoint, 2, 5, 1, REVERSE);

  // initialize the variables we're linked to
  Input = 50;
  Setpoint = 100;

  unsigned long ms = 0;

  bool flag = false;

  // turn the PID on
  myPID.Begin(MANUAL, ms);
  for (int i = 0; i < 1000; i++) {
    ms += 199;
    if (myPID.Compute(ms)) {
      flag = true;
      Input = Input + (Input - Output) / 25.6;
    }
  }

  CHECK(flag == false);
}
