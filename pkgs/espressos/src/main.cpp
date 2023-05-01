#include <ArduinoOTA.h>
#include <LittleFS.h>
#include <WiFi.h>
#include <cstdint>
#include <dimmable_light.h>

#include "api.hpp"
#include "boiler.hpp"
#include "cachedpin.hpp"
#include "config.hpp"
#include "effects.hpp"
#include "fsm/brew.hpp"
#include "fsm/fsmlist.hpp"
#include "fsm/machine.hpp"
#include "logger.hpp"
#include "ota.hpp"
#include "pressure.hpp"
#include "proto/api.h"
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

// Watch variables for change and propagate to hardware/API
static Effects effects;
static Effects apiEffects;

// Re-use loop event on every iteration
static LoopEvent loopEvent;
static TimeEvent timeEvent;

void setup() {
  // Turn on board power LED
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  // Set up logging
  {
    Serial.begin(115200);

    auto multiLogger = new MultiLogger();
    multiLogger->add(new SerialLogger());
    multiLogger->add(&apiServer);
    setLogger(multiLogger);
  }

  // Initialise the FSM
  fsm_list::start();

  // Boiler
  boiler.setup();
  effects.createEffect<int>(
      []() { return MachineState::current_state_ptr->getSetPoint(); },
      [](int setpoint) { boiler.SetSetPoint(setpoint); });

  // Solenoid
  solenoid.setup();
  effects.createEffect<PinStatus>(
      []() { return MachineState::current_state_ptr->getSolenoid(); },
      [](PinStatus pinStatus) { solenoid.digitalWrite(pinStatus); });

  // Pump dimming
  DimmableLight::setSyncPin(PUMP_DIMMER_ZC);
  DimmableLight::begin();
  effects.createEffect<uint8_t>(
      []() { return MachineState::current_state_ptr->getPump(); },
      [](uint8_t pumpIntensity) { pump.setBrightness(pumpIntensity); });

  // Set API server states
  {
    LittleFS.begin();

    apiEffects.createEffect<int>(
        []() { return MachineState::current_state_ptr->getSetPoint(); },
        [](int setpoint) {
          apiServer.setSetpoint(setpoint);
          apiServer.broadcastState();
        });

    // Set boiler temp in API server
    apiEffects.createEffect<TempReading>(
        []() { return boiler.getTemp(); },
        [](TempReading temp) { apiServer.setBoilerTemp(temp); });

    // Set pressure in API server
    apiEffects.createEffect<PressureSensorResult_t>(
        []() { return brewPressure.Read(); },
        [](PressureSensorResult_t pressure) {
          apiServer.setPressure(pressure);
        });

    apiEffects.createEffect<uint32_t>(
        []() { return BrewState::current_state_ptr->getShotStartTime(); },
        [](uint32_t ts) { apiServer.setShotTimerStart(ts); });

    apiEffects.createEffect<uint32_t>(
        []() { return BrewState::current_state_ptr->getShotStopTime(); },
        [](uint32_t ts) { apiServer.setShotTimerStop(ts); });

    // Set state update interval
    apiEffects.createEffect<long>(
        []() {
          return MachineState::current_state_ptr->getStateUpdateInterval();
        },
        [](long updateInterval) {
          apiServer.setStateUpdateInterval(updateInterval);
        });

    // Set mode in API server
    apiEffects.createEffect<MachineMode>(
        []() { return MachineState::current_state_ptr->getMode(); },
        [](MachineMode mode) {
          apiServer.setMachineMode(mode);
          apiServer.broadcastState();
        });

    pConfig->setup();

    WiFi.mode(WIFI_STA);
    WiFi.begin(SSID_NAME, SSID_PASWORD);

    // Set up OTA
    beginOTA(2040);

    // Start listening
    apiServer.begin();
  }
}

void loop() {
  // Run FSM loops
  unsigned long now = millis();
  loopEvent.timestamp = now;
  timeEvent.timestamp = now;
  send_event(loopEvent);
  send_event(timeEvent);

  effects.loop();
  apiEffects.loop();

  boiler.tick();

  handleOTA();
  apiServer.loop();
}
