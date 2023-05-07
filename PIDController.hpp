#pragma once

enum PIDControllerDirection { DIRECT, REVERSE };
enum PIDControllerMode { MANUAL, AUTOMATIC };
enum PIDProportionalOn { P_ON_M, P_ON_E };

class PIDController {

public:
  // constructor.  links the PID to the Input, Output, and
  // Setpoint.  Initial tuning parameters are
  // also set here. (overload for specifying proportional mode)
  PIDController(double *, double *, double *, double, double, double,
                PIDProportionalOn, PIDControllerDirection);

  // constructor.  links the PID to the Input, Output, and
  // Setpoint.  Initial tuning parameters are
  // also set here
  PIDController(double *, double *, double *, double, double, double,
                PIDControllerDirection);

  // Sets initial state relative to current time
  void Begin(unsigned long);
  // Begin in mode with timestamp
  void Begin(PIDControllerMode, unsigned long);

  // sets PID to either Manual (0) or Auto (non-0)
  void SetMode(PIDControllerMode Mode);

  // performs the PID calculation.  it should be
  // called every time loop() cycles. ON/OFF and
  // calculation frequency can be set using SetMode
  // SetSampleTime respectively
  bool Compute(unsigned long);

  // clamps the output to a specific range. 0-255 by default, but
  // it's likely the user will want to change this depending on
  // the application
  void SetOutputLimits(double, double);

  // While most users will set the tunings once in the
  // constructor, this function gives the user the option
  // of changing tunings during runtime for Adaptive control
  void SetTunings(double, double, double);
  // overload for specifying proportional mode
  void SetTunings(double, double, double, PIDProportionalOn);

  // Sets the Direction, or "Action" of the controller. DIRECT
  // means the output will increase when error is positive. REVERSE
  // means the opposite.  it's very unlikely that this will be
  // needed once it is set in the constructor.
  void SetControllerDirection(PIDControllerDirection);

  // sets the frequency, in Milliseconds, with which
  // the PID calculation is performed.  default is 100
  void SetSampleTime(int);

  // These functions query the pid for interal values.
  // they were created mainly for the pid front-end,
  // where it's important to know what is actually
  // inside the PID.
  double GetKp() const;
  double GetKi() const;
  double GetKd() const;
  PIDControllerMode GetMode();
  PIDControllerDirection GetDirection();

private:
  void Initialize();

  // we'll hold on to the tuning parameters in user-entered format for display
  // purposes
  double dispKp;
  double dispKi;
  double dispKd;

  double kp; // (P)roportional Tuning Parameter
  double ki; // (I)ntegral Tuning Parameter
  double kd; // (D)erivative Tuning Parameter

  PIDControllerDirection controllerDirection;
  PIDProportionalOn pOn;
  PIDControllerMode mode;

  // Pointers to the Input, Output, and Setpoint variables
  // This creates a hard link between the variables and the
  double *myInput;
  double *myOutput;

  // PID, freeing the user from having to constantly tell us
  // what these values are.  with pointers we'll just know.
  double *mySetpoint;

  unsigned long lastTime;
  double outputSum, lastInput;

  unsigned long SampleTime;
  double outMin, outMax;
};
