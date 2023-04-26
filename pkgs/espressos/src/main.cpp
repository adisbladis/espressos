#include <ArduinoOTA.h>
#include <LittleFS.h>
#include <WiFi.h>
#include <dimmable_light.h>

#include "api.hpp"
#include "boiler.hpp"
#include "cachedpin.hpp"
#include "config.hpp"
#include "fsm/fsmlist.hpp"
#include "fsm/machine.hpp"
#include "logger.hpp"
#include "ota.hpp"
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
  beginOTA(2040);

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
  bool isBrewing = MachineState::is_in_state<Brewing>();
  bool isPumping = MachineState::is_in_state<Pumping>();
  bool isOff = MachineState::is_in_state<Off>();
  bool isPanic = MachineState::is_in_state<Panic>();
  bool isSteaming = MachineState::is_in_state<Steaming>();

  auto machineState = MachineState::current_state_ptr;

  // Only open solenoid if we're brewing
  solenoid.digitalWrite(machineState->getSolenoid());

  // Only run pump if we're brewing or pumping
  if (isBrewing || isPumping) {
    pump.setBrightness(255);
  } else {
    pump.setBrightness(0);
  }

  // Set boiler setpoint
  {
    auto setpoint = machineState->getSetPoint();
    boiler.SetSetPoint(setpoint);
    apiServer.setSetpoint(setpoint);
  }

  // Send callbacks at 1s intervals normally, 100ms intervals while brewing
  apiServer.setStateUpdateInterval(machineState->getStateUpdateInterval());

  // Run control loops
  {
    if (boiler.tick()) {
      apiServer.setBoilerTemp(boiler.getTemp());
    };

    auto pressure = brewPressure.Read();
    apiServer.setPressure(pressure);
  }

  handleOTA();

  apiServer.loop();
}
