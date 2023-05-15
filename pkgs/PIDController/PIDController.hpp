#pragma once

enum PIDControllerDirection { DIRECT, REVERSE };
enum PIDControllerMode { MANUAL, AUTOMATIC };
enum PIDControllerProportionalOn { P_ON_M, P_ON_E };

template <typename T> struct PIDControllerTunings {
public:
  T kp; // (P)roportional Tuning Parameter
  T ki; // (I)ntegral Tuning Parameter
  T kd; // (D)erivative Tuning Parameter
};

// A lightweight version of std::pair that works on embedded
template <typename M, typename N> struct PIDControllerPair {
  M lhs;
  N rhs;

  PIDControllerPair(M lhs, N rhs) : lhs(lhs), rhs(rhs){};

  M first() { return lhs; };
  M first() const { return lhs; };

  M second() { return lhs; };
  M second() const { return lhs; };
};

template <typename IOT, typename TuningT, typename TimestampT>
class PIDController {

public:
  // constructor.  links the PID to the Input, Output, and
  // Setpoint.  Initial tuning parameters are
  // also set here. (overload for specifying proportional mode)
  //
  // The parameters specified here are those for for which we can't set up
  // reliable defaults, so we need to have the user set them.
  PIDController(IOT Setpoint, TuningT Kp, TuningT Ki, TuningT Kd,
                PIDControllerProportionalOn POn,
                PIDControllerDirection ControllerDirection)
      : mode(MANUAL), // Start disabled
        setpoint(Setpoint), lastTime(0), outputSum(0), lastOutput(0),
        lastInput(0),
        SampleTime(100) // // default Controller Sample Time is 0.1 seconds
  {
    // default output limit corresponds to
    // the arduino pwm limits
    PIDController::SetOutputLimits(0, 255);

    PIDController::SetControllerDirection(ControllerDirection);
    PIDController::SetTunings(Kp, Ki, Kd, POn);
  };

  // constructor.  links the PID to the Input, Output, and
  // Setpoint.  Initial tuning parameters are
  // also set here
  //
  // To allow backwards compatability for v1.1, or for people that just want to
  // use Proportional on Error without explicitly saying so
  PIDController(IOT Setpoint, TuningT Kp, TuningT Ki, TuningT Kd,
                PIDControllerDirection ControllerDirection)
      : PIDController::PIDController(Setpoint, Kp, Ki, Kd, P_ON_E,
                                     ControllerDirection) {}

  // Sets initial state relative to current time
  void Begin(TimestampT now) { lastTime = now - SampleTime; };
  // Begin in mode with timestamp
  void Begin(PIDControllerMode mode, TimestampT now) {
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
  inline PIDControllerMode GetMode() { return mode; };

  void SetSetpoint(IOT Setpoint) { setpoint = Setpoint; };
  IOT GetSetpoint(IOT Setpoint) { return setpoint; };

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
  bool Compute(TimestampT now, IOT Input, IOT *Output,
               bool forceCompute = false) {
    if (mode != AUTOMATIC) {
      return false;
    }

    bool doCompute = forceCompute || (now - lastTime) >= SampleTime;

    if (doCompute) {
      /*Compute all the working error variables*/
      IOT error = setpoint - Input;
      IOT dInput = (Input - lastInput);
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
      IOT output;
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
      *Output = output;

      /*Remember some variables for next time*/
      lastInput = Input;
      lastOutput = output;
      lastTime = now;

      return true;
    }

    *Output = lastOutput;

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
  void SetOutputLimits(TuningT Min, TuningT Max) {
    if (Min >= Max) {
      return;
    }
    outMin = Min;
    outMax = Max;

    if (mode == AUTOMATIC) {
      if (outputSum > outMax) {
        outputSum = outMax;
      } else if (outputSum < outMin) {
        outputSum = outMin;
      }
    }
  };

  PIDControllerPair<TuningT, TuningT> GetOutputLimits() {
    return PIDControllerPair<TuningT, TuningT>(outMin, outMax);
  };

  // While most users will set the tunings once in the
  // constructor, this function gives the user the option
  // of changing tunings during runtime for Adaptive control
  //
  // Set Tunings using the last-rembered POn setting
  void SetTunings(TuningT Kp, TuningT Ki, TuningT Kd) {
    SetTunings(Kp, Ki, Kd, pOn);
  };

  // overload for specifying proportional mode
  //
  // This function allows the controller's dynamic performance to be adjusted.
  // it's called automatically from the constructor, but tunings can also
  // be adjusted on the fly during normal operation
  void SetTunings(TuningT Kp, TuningT Ki, TuningT Kd,
                  PIDControllerProportionalOn POn) {
    if (Kp < 0 || Ki < 0 || Kd < 0) {
      return;
    }

    pOn = POn;

    dispKp = Kp;
    dispKi = Ki;
    dispKd = Kd;

    TuningT SampleTimeInSec = (static_cast<TuningT>(SampleTime)) / 1000;
    kp = Kp;
    ki = Ki * SampleTimeInSec;
    kd = Kd / SampleTimeInSec;

    if (controllerDirection == REVERSE) {
      kp = (0 - kp);
      ki = (0 - ki);
      kd = (0 - kd);
    }
  };

  // This function query the pid for interal values.
  // it was created mainly for the pid front-end,
  // where it's important to know what is actually
  // inside the PID.
  //
  // Just because you set the Kp=-1 doesn't mean it actually happened.  these
  // functions query the internal state of the PID.  they're here for display
  // purposes.  this are the functions the PID Front-end uses for example
  inline PIDControllerTunings<TuningT> GetTunings() const {
    return PIDControllerTunings<TuningT>{dispKp, dispKi, dispKd};
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
  inline PIDControllerDirection GetDirection() const {
    return controllerDirection;
  };

  // sets the frequency, in Milliseconds, with which
  // the PID calculation is performed.  default is 100
  //
  // sets the period, in Milliseconds, at which the calculation is performed
  void SetSampleTime(int NewSampleTime) {
    if (NewSampleTime > 0) {
      TuningT ratio = static_cast<TuningT>(NewSampleTime) /
                      static_cast<TuningT>(SampleTime);
      ki *= ratio;
      kd /= ratio;
      SampleTime = static_cast<TimestampT>(NewSampleTime);
    }
  };
  inline TimestampT GetSampleTime() const { return SampleTime; };

private:
  // does all the things that need to happen to ensure a bumpless transfer
  // from manual to automatic mode.
  void Initialize() {
    outputSum = lastOutput;

    if (outputSum > outMax) {
      outputSum = outMax;
    } else if (outputSum < outMin) {
      outputSum = outMin;
    }
  };

  // we'll hold on to the tuning parameters in user-entered format for display
  // purposes
  TuningT dispKp;
  TuningT dispKi;
  TuningT dispKd;

  TuningT kp; // (P)roportional Tuning Parameter
  TuningT ki; // (I)ntegral Tuning Parameter
  TuningT kd; // (D)erivative Tuning Parameter

  PIDControllerDirection controllerDirection;
  PIDControllerProportionalOn pOn;
  PIDControllerMode mode;

  IOT setpoint;

  TimestampT lastTime;
  IOT outputSum, lastOutput, lastInput;

  TimestampT SampleTime;
  IOT outMin, outMax;
};
