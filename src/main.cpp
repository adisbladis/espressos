#include "LittleFS.h"
#include <ArduinoOTA.h>
#include <WiFi.h>
#include <dimmable_light.h>

#include "api.hpp"
#include "boiler.hpp"
#include "cachedpin.hpp"
#include "fsm/fsmlist.hpp"
#include "pressure.hpp"
#include "websocket.hpp"

// Hardware IO
static BoilerPID boiler(BOILER_SSR_PIN, BOILER_MAX31865_SPI_PIN,
                        BOILER_SPI_CLASS);
static PressureSensor brewPressure(PRESSURE_SENSOR_PIN, PRESSURE_SENSOR_BAR,
                                   PRESSURE_SENSOR_RANGE);
static DimmableLight pump(PUMP_DIMMER_OUT);
static CachedOutputPin solenoid(BREW_SOLENOID_PIN);

static APIServer apiServer = APIServer(HTTP_PORT);

void setup() {
  Serial.begin(115200);

  fsm_list::start();

  boiler.setup();
  solenoid.setup();

  DimmableLight::setSyncPin(PUMP_DIMMER_ZC);
  DimmableLight::begin();

  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID_NAME, SSID_PASWORD);

  LittleFS.begin();

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
  // Only open solenoid if we're brewing
  if (MachineState::is_in_state<Brewing>()) {
    solenoid.digitalWrite(HIGH);
  } else {
    solenoid.digitalWrite(LOW);
  }

  // Only run pump if we're brewing or pumping
  if (MachineState::is_in_state<Brewing>() ||
      MachineState::is_in_state<Pumping>()) {
    pump.setBrightness(255);
  } else {
    pump.setBrightness(0);
  }

  // Set boiler setpoint based on mode
  if (MachineState::is_in_state<Off>() || MachineState::is_in_state<Panic>()) {
    boiler.SetSetPoint(0);
  } else if (MachineState::is_in_state<Steaming>()) {
    boiler.SetSetPoint(90); // TODO: From config
  } else {
    boiler.SetSetPoint(40); // TODO: From config
  }

  // Send callbacks at 1s intervals normally, 100ms intervals while brewing
  if (MachineState::is_in_state<Brewing>()) {
    apiServer.setStateUpdateInterval(STATE_UPDATE_INTERVAL_BREW);
  } else {
    apiServer.setStateUpdateInterval(STATE_UPDATE_INTERVAL);
  }

  // Run control loops
  {
    if (boiler.tick()) {
      TempReading temp = boiler.getTemp();
      apiServer.setBoilerTemp(temp);
    };

    float pressure = brewPressure.Read();
    apiServer.setPressure(pressure);
  }

  ArduinoOTA.handle();
  apiServer.loop();
}
