#include "LittleFS.h"
#include "boiler.hpp"
#include "pressure.hpp"
#include "websocket.hpp"
#include <Adafruit_MAX31865.h>
#include <ArduinoOTA.h>
#include <PID_v1.h>
#include <WiFi.h>
#include <dimmable_light.h>

#include "api.hpp"

BoilerPID boiler(BOILER_SSR_PIN, BOILER_MAX31865_SPI_PIN, BOILER_SPI_CLASS);
PressureSensor brewPressure(PRESSURE_SENSOR_PIN, PRESSURE_SENSOR_BAR,
                            PRESSURE_SENSOR_RANGE);

// Pump output
DimmableLight light(PUMP_DIMMER_OUT);

static APIServer apiServer = APIServer(HTTP_PORT);

void setup() {
  Serial.begin(115200);

  boiler.setup();
  boiler.SetSetPoint(40);

  DimmableLight::setSyncPin(PUMP_DIMMER_ZC);
  DimmableLight::begin();
  light.setBrightness(255); // Full on

  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID_NAME, SSID_PASWORD);

  { LittleFS.begin(); }

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

  apiServer.begin();
}

void loop() {
  {
    if (boiler.tick()) {
      TempReading temp = boiler.getTemp();

      if (temp.fault) {
        Serial.print("ERROR 0x");
        Serial.println(temp.fault, HEX);
        Serial.println(temp.errorMessage());
      }

      // Serial.print("Temp: ");
      // Serial.println(temp.temp);
    };
  }

  {
    float pressure = brewPressure.Read();
    // Serial.print("Pressure: ");
    // Serial.println(pressure);
  }

  { ArduinoOTA.handle(); }

  { apiServer.loop(); }
}
