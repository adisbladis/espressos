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
#include "interval_callback.hpp"
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

static StateUpdate<ERROR_MESSAGE_SIZE, ERROR_MESSAGE_SIZE> stateUpdateMessage;
static IntervalCallback apiServerBroadcastCallback(STATE_UPDATE_INTERVAL);

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
        [](int setpoint) { stateUpdateMessage.set_setpoint(setpoint); });

    // Set boiler temp in API server
    apiEffects.createEffect<TempReading>(
        []() { return boiler.getTemp(); },
        [](TempReading temp) {
          auto boilerTempMsg = stateUpdateMessage.mutable_boilerTemp();

          if (temp.fault) {
            auto fs = boilerTempMsg.get_error();

            auto errorMessage = temp.errorMessage();
            fs.set(errorMessage, strlen(errorMessage));
            boilerTempMsg.set_error(fs);
          } else {
            boilerTempMsg.set_value(temp.temp);
          }

          stateUpdateMessage.set_boilerTemp(boilerTempMsg);
        },
        false); // Update cadence too high to use as update trigger

    // Set pressure in API server
    apiEffects.createEffect<PressureSensorResult_t>(
        []() { return brewPressure.Read(); },
        [](PressureSensorResult_t pressure) {
          auto pressureMsg = stateUpdateMessage.mutable_pressure();

          if (pressure.hasError()) {
            auto fs = pressureMsg.get_error();
            fs.set("Value out of bounds", 19);
            pressureMsg.set_error(fs);
          } else {
            pressureMsg.set_value(pressure.getValue());
          }

          stateUpdateMessage.set_pressure(pressureMsg);
        },
        false); // Update cadence too high to use as update trigger

    apiEffects.createEffect<uint32_t>(
        []() { return BrewState::current_state_ptr->getShotStartTime(); },
        [](uint32_t ts) {
          auto shotTimer = stateUpdateMessage.mutable_shot_timer();
          shotTimer.set_start(ts);
          stateUpdateMessage.set_shot_timer(shotTimer);
        });

    apiEffects.createEffect<uint32_t>(
        []() { return BrewState::current_state_ptr->getShotStopTime(); },
        [](uint32_t ts) {
          auto shotTimer = stateUpdateMessage.mutable_shot_timer();
          shotTimer.set_stop(ts);
          stateUpdateMessage.set_shot_timer(shotTimer);
        });

    // Set state update interval
    apiEffects.createEffect<unsigned long>(
        []() {
          return MachineState::current_state_ptr->getStateUpdateInterval();
        },
        [](unsigned long updateInterval) {
          apiServerBroadcastCallback.setInterval(updateInterval);
        },
        false); // The update interval is irrelevant to the API server

    // Set current time
    apiEffects.createEffect<unsigned long>(
        []() { return MachineState::current_state_ptr->getTimestamp(); },
        [](unsigned long timestamp) {
          stateUpdateMessage.set_millis(timestamp);
        },
        false); // The update interval is irrelevant to the API server

    // Set mode in API server
    apiEffects.createEffect<MachineMode>(
        []() { return MachineState::current_state_ptr->getMode(); },
        [](MachineMode mode) { stateUpdateMessage.set_mode(mode); });

    // If any watched effects triggered send update to clients
    apiEffects.onTriggered([]() { apiServerBroadcastCallback.reset(); });

    // Set up state broadcast callback
    apiServerBroadcastCallback.setCallback(
        []() { apiServer.broadcastState(stateUpdateMessage); });

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
  unsigned long now = millis();

  // Run FSM loops
  timeEvent.timestamp = now;
  send_event(timeEvent);
  loopEvent.timestamp = now;
  send_event(loopEvent);

  // Reconcile hardware with FSM state
  effects.loop();

  // Set API state
  apiEffects.loop();

  // Run boiler PID loop
  boiler.tick();

  // Handle Arduino OTA
  handleOTA();

  // Handle websocket API
  apiServer.loop();

  // Send state updates on value change/interval
  apiServerBroadcastCallback.loop(now);
}
