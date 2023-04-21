#include <ArduinoOTA.h>
#include <LittleFS.h>
#include <WiFi.h>
#include <dimmable_light.h>

#include "api.hpp"
#include "boiler.hpp"
#include "cachedpin.hpp"
#include "config.hpp"
#include "fsm/fsmlist.hpp"
#include "logger.hpp"
#include "pressure.hpp"
#include "websocket.hpp"

// Hardware IO
static BoilerPID boiler(BOILER_SSR_PIN, BOILER_MAX31865_SPI_PIN,
                        BOILER_SPI_CLASS);
static PressureSensor brewPressure(PRESSURE_SENSOR_PIN, PRESSURE_SENSOR_BAR,
                                   PRESSURE_SENSOR_RANGE);
static DimmableLight pump(PUMP_DIMMER_OUT);
static CachedOutputPin solenoid(BREW_SOLENOID_PIN);

static PersistedConfig *pConfig = new PersistedConfig();
static APIServer apiServer = APIServer(HTTP_PORT, pConfig);

void setup() {
  // Turn on board power LED
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  Serial.begin(115200);

  fsm_list::start();

  boiler.setup();
  solenoid.setup();

  DimmableLight::setSyncPin(PUMP_DIMMER_ZC);
  DimmableLight::begin();

  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID_NAME, SSID_PASWORD);

  LittleFS.begin();

  pConfig->setup();

  // Set up OTA
  {
    ArduinoOTA.setPort(2040);

    ArduinoOTA.onStart([]() { logger->log(LogLevel::DEBUG, "OTA start"); });
    ArduinoOTA.onEnd([]() { logger->log(LogLevel::DEBUG, "OTA end"); });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
      // logger->log(LogLevel::DEBUG, "OTA progress: %u%%", pct);
    });
    ArduinoOTA.onError([](ota_error_t error) {
      const char *message = "Unknown error";

      switch (error) {
      case OTA_AUTH_ERROR:
        message = "Auth Failed";
      case OTA_BEGIN_ERROR:
        message = "Begin Failed";
      case OTA_CONNECT_ERROR:
        message = "Connect Failed";
      case OTA_RECEIVE_ERROR:
        message = "Receive Failed";
      case OTA_END_ERROR:
        message = "End Failed";
      }

      logger->log(LogLevel::ERROR, "OTA Error[%u]: %s", error, message);
    });

    ArduinoOTA.begin();
  }

  // Start listening
  apiServer.begin();

  // Set up logging
  {
    auto multiLogger = new MultiLogger();
    multiLogger->add(new SerialLogger());
    multiLogger->add(&apiServer);
    setLogger(multiLogger);
  }
}

void loop() {
  auto config = pConfig->getConfig();

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
    boiler.SetSetPoint(config.get_steamSetPoint());
  } else {
    boiler.SetSetPoint(config.get_setpoint());
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

      // if (temp.fault) {
      //   Serial.println("Foo??");
      //   // TODO: send panic event via fifo
      //   send_event(PanicEvent());
      // }

      apiServer.setBoilerTemp(temp);
    };

    auto pressure = brewPressure.Read();
    apiServer.setPressure(pressure);
  }

  ArduinoOTA.handle();
  apiServer.loop();
}
