/********************************************************
 * PID RelayOutput Example
 * Same as basic example, except that this time, the output
 * is going to a digital pin which (we presume) is controlling
 * a relay.  the pid is designed to Output an analog value,
 * but the relay can only be On/Off.
 *
 *   to connect them together we use "time proportioning
 * control"  it's essentially a really slow version of PWM.
 * first we decide on a window size (5000mS say.) we then
 * set the pid to adjust its output between 0 and that window
 * size.  lastly, we add some logic that translates the PID
 * output into "Relay On Time" with the remainder of the
 * window being "Relay Off Time"
 ********************************************************/

#include <PIDController.hpp>

#define PIN_INPUT 0
#define RELAY_PIN 6

// Define Variables we'll be connecting to
double Input, Output;
static constexpr double Setpoint = 100;

// Specify the links and initial tuning parameters
double Kp = 2, Ki = 5, Kd = 1;
PIDController<double, double> myPID(Setpoint, Kp, Ki, Kd,
                                    PIDControllerDirection::DIRECT);

int WindowSize = 5000;
unsigned long windowStartTime;

void setup() {
  windowStartTime = millis();

  pinMode(RELAY_PIN, OUTPUT);

  // tell the PID to range between 0 and the full window size
  myPID.SetOutputLimits(0, WindowSize);

  // turn the PID on
  myPID.Begint(PIDControllerMode::AUTOMATIC, millis());
}

void loop() {
  Input = analogRead(PIN_INPUT);
  myPID.Compute(millis(), Input, &Output);

  /************************************************
   * turn the output pin on/off based on pid output
   ************************************************/
  if (millis() - windowStartTime >=
      WindowSize) { // time to shift the Relay Window
    windowStartTime += WindowSize;
  }
  if (Output < millis() - windowStartTime) {
    digitalWrite(RELAY_PIN, HIGH);
  } else {
    digitalWrite(RELAY_PIN, LOW);
  }
}
