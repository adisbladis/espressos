#pragma once

#include <Adafruit_MAX31865.h>
#include <PID_v1.h>

struct TempReading {
  double temp;
  uint8_t fault; // Fault identifier

  String errorMessage() {
    String message;
    if (!fault) {
      return message;
    }

    if (fault & MAX31865_FAULT_HIGHTHRESH) {
      message += "RTD High Threshold\n";
    }
    if (fault & MAX31865_FAULT_LOWTHRESH) {
      message += "RTD Low Threshold\n";
    }
    if (fault & MAX31865_FAULT_REFINLOW) {
      message += "REFIN- > 0.85 x Bias";
    }
    if (fault & MAX31865_FAULT_REFINHIGH) {
      message += "REFIN- < 0.85 x Bias - FORCE- open\n";
    }
    if (fault & MAX31865_FAULT_RTDINLOW) {
      message += "RTDIN- < 0.85 x Bias - FORCE- open\n";
    }
    if (fault & MAX31865_FAULT_OVUV) {
      message += "Under/Over voltage\n";
    }

    return message;
  }
};

class BoilerPID {
private:
  double outputPin;
  Adafruit_MAX31865 thermo;
  double Setpoint, Input, Output;
  double Kp = 2, Ki = 60, Kd = 15;
  int WindowSize = 50;
  unsigned long windowStartTime;
  PID pid;

public:
  BoilerPID(double relayPin, double max31865SPIPin, SPIClass *theSPI);

  void setup();
  void SetSetPoint(double setPoint);

  double GetSetPoint();

  struct TempReading getTemp();

  void SetTunings(double Kp, double Ki, double Kd);

  TempReading tick();
};
