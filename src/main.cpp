#include "LittleFS.h"
#include <Adafruit_MAX31865.h>
#include <ArduinoOTA.h>
#include <PID_v1.h>
#include <WiFi.h>
#include <dimmable_light.h>
#include "boiler.h"

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

  int ReadRaw() { return analogRead(inputPin); }

  float Bar(int raw) { return (raw - floor) * sensorBar / ceil; }

  float Read() { return Bar(ReadRaw()); }
};

BoilerPID boiler = BoilerPID(0, 13, &SPI1);
PressureSensor brewPressure = PressureSensor(26, 20.6843);

// Pump output
const int zcPin = 4;
const int dimPin = 5;
DimmableLight light(dimPin);

void setup() {
  Serial.begin(115200);
  while (!Serial) {
  }

  boiler.setup();
  boiler.SetSetPoint(40);

  DimmableLight::setSyncPin(zcPin);
  DimmableLight::begin();

  WiFi.mode(WIFI_STA);
  WiFi.begin("kombu", "giftedwhale");

  { LittleFS.begin(); }

  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
  }

  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Set up OTA
  {
    ArduinoOTA.setPort(2040);

    ArduinoOTA.onStart([]() { Serial.println("Start"); });
    ArduinoOTA.onEnd([]() { Serial.println("\nEnd"); });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });
    ArduinoOTA.onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR)
        Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR)
        Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR)
        Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR)
        Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR)
        Serial.println("End Failed");
    });

    ArduinoOTA.begin();
  }
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

  {
    // Serial.println("Handling OTA");
    ArduinoOTA.handle();
  }
}
