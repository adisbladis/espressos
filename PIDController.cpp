/**********************************************************************************************
 * Arduino PID Library - Version 1.2.1
 * by Brett Beauregard <br3ttb@gmail.com> brettbeauregard.com
 *
 * This Library is licensed under the MIT License
 **********************************************************************************************/

#include "PIDController.hpp"

// The parameters specified here are those for for which we can't set up
// reliable defaults, so we need to have the user set them.
PIDController::PIDController(double *Input, double *Output, double *Setpoint,
                             double Kp, double Ki, double Kd,
                             PIDProportionalOn POn,
                             PIDControllerDirection ControllerDirection) {
  myOutput = Output;
  myInput = Input;
  mySetpoint = Setpoint;
  mode = MANUAL;

  // default output limit corresponds to
  PIDController::SetOutputLimits(0, 255);
  // the arduino pwm limits

  SampleTime = 100; // default Controller Sample Time is 0.1 seconds

  PIDController::SetControllerDirection(ControllerDirection);
  PIDController::SetTunings(Kp, Ki, Kd, POn);
}

// To allow backwards compatability for v1.1, or for people that just want to
// use Proportional on Error without explicitly saying so
PIDController::PIDController(double *Input, double *Output, double *Setpoint,
                             double Kp, double Ki, double Kd,
                             PIDControllerDirection ControllerDirection)
    : PIDController::PIDController(Input, Output, Setpoint, Kp, Ki, Kd, P_ON_E,
                                   ControllerDirection) {}

void PIDController::Begin(unsigned long now) { lastTime = now - SampleTime; }

void PIDController::Begin(PIDControllerMode mode, unsigned long now) {
  Begin(now);
  SetMode(mode);
}

// This, as they say, is where the magic happens.
// This function should be called every time "void loop()" executes.
// The function will decide for itself whether a new pid Output needs to be
// computed. returns true when the output is computed, false when nothing has
// been done.
bool PIDController::Compute(unsigned long now) {
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
}

// This function allows the controller's dynamic performance to be adjusted.
// it's called automatically from the constructor, but tunings can also
// be adjusted on the fly during normal operation
void PIDController::SetTunings(double Kp, double Ki, double Kd,
                               PIDProportionalOn POn) {
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
}

// Set Tunings using the last-rembered POn setting
void PIDController::SetTunings(double Kp, double Ki, double Kd) {
  SetTunings(Kp, Ki, Kd, pOn);
}

// sets the period, in Milliseconds, at which the calculation is performed
void PIDController::SetSampleTime(int NewSampleTime) {
  if (NewSampleTime > 0) {
    double ratio =
        static_cast<double>(NewSampleTime) / static_cast<double>(SampleTime);
    ki *= ratio;
    kd /= ratio;
    SampleTime = static_cast<unsigned long>(NewSampleTime);
  }
}

// This function will be used far more often than SetInputLimits.  while
// the input to the controller will generally be in the 0-1023 range (which is
// the default already,)  the output will be a little different.  maybe they'll
// be doing a time window and will need 0-8000 or something.  or maybe they'll
// want to clamp it from 0-125.  who knows.  at any rate, that can all be done
// here.
void PIDController::SetOutputLimits(double Min, double Max) {
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
}

// Allows the controller Mode to be set to manual (0) or Automatic (non-zero)
// when the transition from manual to auto occurs, the controller is
// automatically initialized
void PIDController::SetMode(PIDControllerMode Mode) {
  /*we just went from manual to auto*/
  if (Mode == AUTOMATIC && mode != AUTOMATIC) {
    PIDController::Initialize();
  }

  mode = Mode;
}

// does all the things that need to happen to ensure a bumpless transfer
// from manual to automatic mode.
void PIDController::Initialize() {
  outputSum = *myOutput;
  lastInput = *myInput;
  if (outputSum > outMax) {
    outputSum = outMax;
  } else if (outputSum < outMin) {
    outputSum = outMin;
  }
}

// The PID will either be connected to a PIDControllerDirection::DIRECT acting
// process (+Output leads to +Input) or a REVERSE acting process(+Output leads
// to -Input.)  we need to know which one, because otherwise we may increase the
// output when we should be decreasing.  This is called from the constructor.
void PIDController::SetControllerDirection(PIDControllerDirection Direction) {
  if (mode == AUTOMATIC && Direction != controllerDirection) {
    kp = (0 - kp);
    ki = (0 - ki);
    kd = (0 - kd);
  }
  controllerDirection = Direction;
}

// Just because you set the Kp=-1 doesn't mean it actually happened.  these
// functions query the internal state of the PID.  they're here for display
// purposes.  this are the functions the PID Front-end uses for example
double PIDController::GetKp() const { return dispKp; }
double PIDController::GetKi() const { return dispKi; }
double PIDController::GetKd() const { return dispKd; }
PIDControllerMode PIDController::GetMode() { return mode; }
PIDControllerDirection PIDController::GetDirection() {
  return controllerDirection;
}
