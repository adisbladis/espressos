#pragma once

#include "cachedpin.hpp"
#include <Adafruit_MAX31865.h>
#include <PIDController.hpp>
#include <cstdint>

struct TempReading {
  int16_t temp;
  uint8_t fault; // Fault identifier

  bool operator==(TempReading const &rhs) const {
    return fault == rhs.fault && temp == rhs.temp;
  }

  bool operator!=(TempReading const &rhs) const { return !(*this == rhs); }

  const char *errorMessage() {
    String message;

    if (!fault) {
      return "";
    }

    message = "RTD Error 0x" + String(fault, HEX) + ": ";

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

    return message.c_str();
  }
};

class BoilerPID {
private:
  CachedOutputPin outputPin;
  Adafruit_MAX31865 thermo;
  float Kp = BOILER_PID_P, Ki = BOILER_PID_I, Kd = BOILER_PID_D;
  int WindowSize = 100;
  unsigned long windowStartTime;
  int32_t Output;
  PIDController<int32_t, float, unsigned long> pid;
  TempReading temp;

public:
  BoilerPID(int relayPin, int max31865SPIPin, SPIClass *theSPI);

  void setup(unsigned long now);
  void SetSetPoint(float setPoint);

  struct TempReading getTemp();

  void SetTunings(float Kp, float Ki, float Kd);

  bool loop(unsigned long now);
};
