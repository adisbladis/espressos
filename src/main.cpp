#include "LittleFS.h"
#include "boiler.h"
#include "pressure.h"
#include <Adafruit_MAX31865.h>
#include <ArduinoOTA.h>
#include <PID_v1.h>
#include <WiFi.h>
#include <dimmable_light.h>

BoilerPID boiler = BoilerPID(0, 13, &SPI1);
PressureSensor brewPressure = PressureSensor(26, 20.6843, 0.8);

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
