#include <Adafruit_MAX31865.h>
#include <PID_v1.h>
#include <dimmable_light.h>

// Thermal readings
#define RREF 430.0 // Value of Rref resistor (PT100 == 430.0, PT1000 == 4300.0)
#define RNOMINAL  100.0 // "Nominal" 0-degrees resistance of the sensor (PT100 == 100.0, PT1000 == 1000.0)

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
    BoilerPID(double relayPin, double max31865SPIPin, SPIClass *theSPI = &SPI): pid(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT), thermo(max31865SPIPin, theSPI) {
      outputPin = relayPin;
    };

    void setup() {
      pinMode(outputPin, OUTPUT);

      thermo.begin(MAX31865_2WIRE);

      windowStartTime = millis();
      pid.SetOutputLimits(0, WindowSize); // PID output range
      pid.SetMode(AUTOMATIC);
    }

    void SetSetPoint(double setPoint) {
      Setpoint = setPoint;
    }

    double GetSetPoint() {
      return Setpoint;
    }

    struct TempReading getTemp() {
      struct TempReading result;

      uint16_t rtd = thermo.readRTD();
      result.temp = thermo.temperature(RNOMINAL, RREF);

      // Check if any errors occured
      result.fault = thermo.readFault();
      thermo.clearFault();

      return result;
    }

    void SetTunings(double Kp, double Ki, double Kd) {
      pid.SetTunings(Kp, Ki, Kd);
    }

    TempReading tick() {
      struct TempReading temp = getTemp();

      // If a hardware error occured while checking temps disable the boiler
      if (temp.fault) {
        pid.SetMode(MANUAL);
        digitalWrite(outputPin, LOW);
        return temp;
      }

      // If a previous error occured we might have disabled the boiler
      if (pid.GetMode() != AUTOMATIC) {
        pid.SetMode(AUTOMATIC);
      }

      Input = temp.temp;
      pid.Compute();

      if (millis() - windowStartTime >= WindowSize) {
        windowStartTime += WindowSize;
      }
      if (Output > 0 && Output < millis() - windowStartTime) {
        digitalWrite(outputPin, HIGH);
      } else {
        digitalWrite(outputPin, LOW);
      }

      return temp;
    }
};

class PressureSensor {
  private:
    int inputPin;
    float sensorBar;
    float floor, ceil;

  public:
    PressureSensor(int inputPin, float sensorBar, float sensorRange = 0.8) {
      this->inputPin = inputPin;
      this->sensorBar = sensorBar;

      // Calculate the usable range of values from the ADC
      // The default value 0.9 means that we "lose" 20% of ADC range
      // as the sensor has a voltage range from 0.5v to 4.5v with a VCC of 5v.
      floor = (1024 - (1024 * sensorRange)) / 2;
      ceil = 1024 - floor;
    }

    int ReadRaw() {
      return analogRead(inputPin);
    }

    float Bar(int raw) {
      return (raw - floor) * sensorBar / ceil;
    }

    float Read() {
      return Bar(ReadRaw());
    }
};

BoilerPID boiler = BoilerPID(0, 13, &SPI1);
PressureSensor brewPressure = PressureSensor(26, 20.6843);

// Pump output
const int zcPin = 4;
const int dimPin = 5;
DimmableLight light(dimPin);

void setup() {
  Serial.begin(115200);

  boiler.setup();
  boiler.SetSetPoint(40);

  DimmableLight::setSyncPin(zcPin);
  DimmableLight::begin();
}

void loop() {
  {
    TempReading temp = boiler.tick();

    if (temp.fault) {
      Serial.print("ERROR 0x");
      Serial.println(temp.fault, HEX);
      Serial.println(temp.errorMessage());
    }

    Serial.print("Temp: ");
    Serial.println(temp.temp);
  }

  {
    float pressure = brewPressure.Read();
    Serial.print("Pressure: ");
    Serial.println(pressure);
  }

  {
    light.setBrightness(255); // Full on
  }
}
