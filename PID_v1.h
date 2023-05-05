#ifndef PID_v1_h
#define PID_v1_h
#define LIBRARY_VERSION 1.2.1

enum PIDControllerDirection { DIRECT, REVERSE };
enum PIDControllerMode { MANUAL, AUTOMATIC };
enum PIDProportionalOn { P_ON_M, P_ON_E };

class PID {

public:
  // commonly used functions
  // **************************************************************************
  PID(double *, double *,
      double *, // * constructor.  links the PID to the Input, Output, and
      double, double, double, PIDProportionalOn,
      PIDControllerDirection); //   Setpoint.  Initial tuning parameters are
                               //   also set here. (overload for specifying
                               //   proportional mode)

  PID(double *, double *,
      double *, // * constructor.  links the PID to the Input, Output, and
      double, double, double,
      PIDControllerDirection); //   Setpoint.  Initial tuning parameters are
                               //   also set here

  void SetMode(PIDControllerMode Mode); // * sets PID to either Manual (0) or Auto (non-0)

  // * performs the PID calculation.  it should be
  //   called every time loop() cycles. ON/OFF and
  //   calculation frequency can be set using SetMode
  //   SetSampleTime respectively
  bool Compute();

  // * clamps the output to a specific range. 0-255 by default, but
  //   it's likely the user will want to change this depending on
  //   the application
  void SetOutputLimits(double, double);

  // available but not commonly used functions
  // ********************************************************
  void SetTunings(
      double, double, // * While most users will set the tunings once in the
      double);        //   constructor, this function gives the user the option
               //   of changing tunings during runtime for Adaptive control
  void SetTunings(double, double, // * overload for specifying proportional mode
                  double, PIDProportionalOn);

  // * Sets the Direction, or "Action" of the controller. DIRECT
  //   means the output will increase when error is positive. REVERSE
  //   means the opposite.  it's very unlikely that this will be
  //   needed once it is set in the constructor.
  void SetControllerDirection(PIDControllerDirection);

  // * sets the frequency, in Milliseconds, with which
  //   the PID calculation is performed.  default is 100
  void SetSampleTime(int);

  // Display functions
  // ****************************************************************
  double GetKp(); // These functions query the pid for interal values.
  double GetKi(); //  they were created mainly for the pid front-end,
  double GetKd(); // where it's important to know what is actually
  PIDControllerMode GetMode(); //  inside the PID.
  PIDControllerDirection GetDirection();          //

private:
  void Initialize();

  double dispKp; // * we'll hold on to the tuning parameters in user-entered
  double dispKi; //   format for display purposes
  double dispKd; //

  double kp; // * (P)roportional Tuning Parameter
  double ki; // * (I)ntegral Tuning Parameter
  double kd; // * (D)erivative Tuning Parameter

  PIDControllerDirection controllerDirection;
  PIDProportionalOn pOn;
  PIDControllerMode mode;

  double *myInput;  // * Pointers to the Input, Output, and Setpoint variables
  double *myOutput; //   This creates a hard link between the variables and the

  // PID, freeing the user from having to constantly tell us
  // what these values are.  with pointers we'll just know.
  double *mySetpoint;

  unsigned long lastTime;
  double outputSum, lastInput;

  unsigned long SampleTime;
  double outMin, outMax;
};
#endif
