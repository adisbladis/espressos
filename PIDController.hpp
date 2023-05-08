#pragma once

enum PIDControllerDirection { DIRECT, REVERSE };
enum PIDControllerMode { MANUAL, AUTOMATIC };
enum PIDProportionalOn { P_ON_M, P_ON_E };

class PIDController {

public:
  // constructor.  links the PID to the Input, Output, and
  // Setpoint.  Initial tuning parameters are
  // also set here. (overload for specifying proportional mode)
  //
  // The parameters specified here are those for for which we can't set up
  // reliable defaults, so we need to have the user set them.
  PIDController(double *Input, double *Output, double *Setpoint, double Kp,
                double Ki, double Kd, PIDProportionalOn POn,
                PIDControllerDirection ControllerDirection) {
    myOutput = Output;
    myInput = Input;
    mySetpoint = Setpoint;
    mode = MANUAL;

    // default output limit corresponds to
    // the arduino pwm limits
    PIDController::SetOutputLimits(0, 255);

    SampleTime = 100; // default Controller Sample Time is 0.1 seconds

    PIDController::SetControllerDirection(ControllerDirection);
    PIDController::SetTunings(Kp, Ki, Kd, POn);
  };

  // constructor.  links the PID to the Input, Output, and
  // Setpoint.  Initial tuning parameters are
  // also set here
  //
  // To allow backwards compatability for v1.1, or for people that just want to
  // use Proportional on Error without explicitly saying so
  PIDController(double *Input, double *Output, double *Setpoint, double Kp,
                double Ki, double Kd,
                PIDControllerDirection ControllerDirection)
      : PIDController::PIDController(Input, Output, Setpoint, Kp, Ki, Kd,
                                     P_ON_E, ControllerDirection) {}

  // Sets initial state relative to current time
  void Begin(unsigned long now) { lastTime = now - SampleTime; };
  // Begin in mode with timestamp
  void Begin(PIDControllerMode mode, unsigned long now) {
    Begin(now);
    SetMode(mode);
  };

  // sets PID to either Manual (0) or Auto (non-0)
  //
  // Allows the controller Mode to be set to manual (0) or Automatic (non-zero)
  // when the transition from manual to auto occurs, the controller is
  // automatically initialized
  void SetMode(PIDControllerMode Mode) {
    /*we just went from manual to auto*/
    if (Mode == AUTOMATIC && mode != AUTOMATIC) {
      PIDController::Initialize();
    }

    mode = Mode;
  };

  // performs the PID calculation.  it should be
  // called every time loop() cycles. ON/OFF and
  // calculation frequency can be set using SetMode
  // SetSampleTime respectively
  //
  // This, as they say, is where the magic happens.
  // This function should be called every time "void loop()" executes.
  // The function will decide for itself whether a new pid Output needs to be
  // computed. returns true when the output is computed, false when nothing has
  // been done.
  bool Compute(unsigned long now) {
    if (mode != AUTOMATIC) {
      return false;
    }

    unsigned long timeChange = (now - lastTime);
    if (timeChange >= SampleTime) {
      /*Compute all the working error variables*/
      double input = *myInput;
      double error = *mySetpoint - input;
      double dInput = (input - lastInput);
      outputSum += (ki * error);

      /*Add Proportional on Measurement, if P_ON_M is specified*/
      if (pOn == P_ON_M) {
        outputSum -= kp * dInput;
      }

      if (outputSum > outMax) {
        outputSum = outMax;
      } else if (outputSum < outMin) {
        outputSum = outMin;
      }

      /*Add Proportional on Error, if P_ON_E is specified*/
      double output;
      if (pOn == P_ON_E) {
        output = kp * error;
      } else {
        output = 0;
      }

      /*Compute Rest of PID Output*/
      output += outputSum - kd * dInput;

      if (output > outMax) {
        output = outMax;
      } else if (output < outMin) {
        output = outMin;
      }
      *myOutput = output;

      /*Remember some variables for next time*/
      lastInput = input;
      lastTime = now;
      return true;
    }
    return false;
  };

  // clamps the output to a specific range. 0-255 by default, but
  // it's likely the user will want to change this depending on
  // the application
  //
  // This function will be used far more often than SetInputLimits.  while
  // the input to the controller will generally be in the 0-1023 range (which is
  // the default already,)  the output will be a little different.  maybe
  // they'll be doing a time window and will need 0-8000 or something.  or maybe
  // they'll want to clamp it from 0-125.  who knows.  at any rate, that can all
  // be done here.
  void SetOutputLimits(double Min, double Max) {
    if (Min >= Max) {
      return;
    }
    outMin = Min;
    outMax = Max;

    if (mode == AUTOMATIC) {
      if (*myOutput > outMax) {
        *myOutput = outMax;
      } else if (*myOutput < outMin) {
        *myOutput = outMin;
      }

      if (outputSum > outMax) {
        outputSum = outMax;
      } else if (outputSum < outMin) {
        outputSum = outMin;
      }
    }
  };

  // While most users will set the tunings once in the
  // constructor, this function gives the user the option
  // of changing tunings during runtime for Adaptive control
  //
  // Set Tunings using the last-rembered POn setting
  void SetTunings(double Kp, double Ki, double Kd) {
    SetTunings(Kp, Ki, Kd, pOn);
  };
  // overload for specifying proportional mode
  //
  // This function allows the controller's dynamic performance to be adjusted.
  // it's called automatically from the constructor, but tunings can also
  // be adjusted on the fly during normal operation
  void SetTunings(double Kp, double Ki, double Kd, PIDProportionalOn POn) {
    if (Kp < 0 || Ki < 0 || Kd < 0) {
      return;
    }

    pOn = POn;

    dispKp = Kp;
    dispKi = Ki;
    dispKd = Kd;

    double SampleTimeInSec = (static_cast<double>(SampleTime)) / 1000;
    kp = Kp;
    ki = Ki * SampleTimeInSec;
    kd = Kd / SampleTimeInSec;

    if (controllerDirection == REVERSE) {
      kp = (0 - kp);
      ki = (0 - ki);
      kd = (0 - kd);
    }
  };

  // Sets the Direction, or "Action" of the controller. DIRECT
  // means the output will increase when error is positive. REVERSE
  // means the opposite.  it's very unlikely that this will be
  // needed once it is set in the constructor.
  //
  // The PID will either be connected to a PIDControllerDirection::DIRECT acting
  // process (+Output leads to +Input) or a REVERSE acting process(+Output leads
  // to -Input.)  we need to know which one, because otherwise we may increase
  // the output when we should be decreasing.  This is called from the
  // constructor.
  void SetControllerDirection(PIDControllerDirection Direction) {
    if (mode == AUTOMATIC && Direction != controllerDirection) {
      kp = (0 - kp);
      ki = (0 - ki);
      kd = (0 - kd);
    }
    controllerDirection = Direction;
  };

  // sets the frequency, in Milliseconds, with which
  // the PID calculation is performed.  default is 100
  //
  // sets the period, in Milliseconds, at which the calculation is performed
  void SetSampleTime(int NewSampleTime) {
    if (NewSampleTime > 0) {
      double ratio =
          static_cast<double>(NewSampleTime) / static_cast<double>(SampleTime);
      ki *= ratio;
      kd /= ratio;
      SampleTime = static_cast<unsigned long>(NewSampleTime);
    }
  };

  // These functions query the pid for interal values.
  // they were created mainly for the pid front-end,
  // where it's important to know what is actually
  // inside the PID.
  //
  // Just because you set the Kp=-1 doesn't mean it actually happened.  these
  // functions query the internal state of the PID.  they're here for display
  // purposes.  this are the functions the PID Front-end uses for example
  double GetKp() const { return dispKp; };
  double GetKi() const { return dispKi; };
  double GetKd() const { return dispKd; };
  PIDControllerMode GetMode() { return mode; };
  PIDControllerDirection GetDirection() { return controllerDirection; };

private:
  // does all the things that need to happen to ensure a bumpless transfer
  // from manual to automatic mode.
  void Initialize() {
    outputSum = *myOutput;
    lastInput = *myInput;
    if (outputSum > outMax) {
      outputSum = outMax;
    } else if (outputSum < outMin) {
      outputSum = outMin;
    }
  };

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
