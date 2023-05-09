#include <ArduinoOTA.h>
#include <LittleFS.h>
#include <PIDController.hpp>
#include <WiFi.h>
#include <cstdint>
#include <dimmable_light.h>
#include <SimpleKalmanFilter.h>

#include "api/handler.hpp"
#include "boiler.hpp"
#include "cachedpin.hpp"
#include "config.hpp"
#include "fsm/brew.hpp"
#include "fsm/events.hpp"
#include "fsm/fsmlist.hpp"
#include "fsm/machine.hpp"
#include "fsm/pump.hpp"
#include "http/api.hpp"
#include "lib/effects.hpp"
#include "lib/interval_callback.hpp"
#include "lib/map.hpp"
#include "logger.hpp"
#include "ota.hpp"
#include "pressure.hpp"
#include "proto/api.h"

// Hardware IO
static BoilerPID boiler(BOILER_SSR_PIN, BOILER_MAX31865_SPI_PIN,
                        BOILER_SPI_CLASS);
static PressureSensor brewPressure(PRESSURE_SENSOR_PIN, PRESSURE_SENSOR_MBAR,
                                   PRESSURE_SENSOR_RANGE);
static DimmableLight pump(PUMP_DIMMER_OUT);
static CachedOutputPin solenoid(BREW_SOLENOID_PIN);

static PersistedConfig pConfig;
static APIHandler apiHandler(&pConfig);
static APIWebServer apiServer = APIWebServer(HTTP_PORT, &apiHandler);

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

  // Run FSM loops on millisecond change
  effects.createEffect<unsigned long>(millis, [](unsigned long now) {
    timeEvent.timestamp = now;
    send_event(timeEvent);

    loopEvent.timestamp = now;
    send_event(loopEvent);
  });

  // Pressure sensor
  {
    static SimpleKalmanFilter pressureKalmanFilter(0.6f, 0.6f, 0.1f);
    static uint16_t smoothPressure = 0;

    effects.createEffect<PressureSensorResult_t>(
        []() { return brewPressure.Read(); },
        [](PressureSensorResult_t pressure) {
          if (pressure.hasError()) {
            send_event(PanicEvent());
            return;
          }

          smoothPressure =
              pressureKalmanFilter.updateEstimate(pressure.getValue());
        });

    effects.createEffect<uint16_t>([]() { return smoothPressure; },
                                   [](uint16_t pressure) {
                                     PressureEvent event;
                                     event.pressure = pressure;
                                     send_event(event);
                                   });
  }

  // Boiler
  boiler.setup(millis());
  effects.createEffect<std::uint16_t>(
      []() { return MachineState::current_state_ptr->getSetPoint(); },
      [](std::uint16_t setpoint) { boiler.SetSetPoint(setpoint); });
  effects.createEffect<unsigned long>(
      []() { return MachineState::current_state_ptr->getTimestamp(); },
      [](unsigned long now) {
        boiler.loop(now); // Run boiler PID loop
      });

  // Solenoid
  solenoid.setup();
  effects.createEffect<PinStatus>(
      []() { return MachineState::current_state_ptr->getSolenoid(); },
      [](PinStatus pinStatus) { solenoid.digitalWrite(pinStatus); });

  // Pump dimming
  {
    DimmableLight::setSyncPin(PUMP_DIMMER_ZC);
    DimmableLight::begin();

    static PumpTarget lastPumpTarget = (PumpTarget){PumpMode::POWER, 0};
    static PIDController<int32_t, float, unsigned long> pressureProfilePID(
        0, 1, 7, 0, DIRECT);

    pressureProfilePID.Begin(AUTOMATIC, millis());

    static uint8_t pumpPower = 0;

    effects.createEffect<uint8_t>(
        []() { return pumpPower; },
        [](uint8_t power) { pump.setBrightness(power); });

    // TODO: Reset PID internals on mode switch

    // React to state machine pump changes
    effects.createEffect<PumpTarget>(
        []() { return MachineState::current_state_ptr->getPump(); },
        [](PumpTarget pumpTarget) {
          switch (pumpTarget.mode) {
          case POWER:
            // Remap value into uint8 range used by dimmer and apply
            pumpPower = map<uint16_t>(pumpTarget.value, 0, 65535, 0, 255);
            break;

          case PRESSURE:
            pressureProfilePID.SetSetpoint(pumpTarget.value);
            break;

          default:
            // We don't know what we're doing, the only safe thing to do is to
            // stop.
            send_event(PanicEvent());
          }

          lastPumpTarget = pumpTarget;
        });

    // Run pump pressure profiling PID loop
    effects.createEffect<unsigned long>(
        []() { return MachineState::current_state_ptr->getTimestamp(); },
        [](unsigned long timestamp) {
          if (lastPumpTarget.mode != PumpMode::PRESSURE) {
            return;
          }

          int32_t output;
          if (pressureProfilePID.Compute(
                  timestamp, MachineState::current_state_ptr->getPressure(),
                  &output)) {
            pumpPower = output;
          }
        });
  }

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
        false); // Update cadence too high to use as a message trigger

    // Set mode in API server
    apiEffects.createEffect<MachineMode>(
        []() { return MachineState::current_state_ptr->getMode(); },
        [](MachineMode mode) { stateUpdateMessage.set_mode(mode); });

    // If any watched effects triggered send update to clients
    apiEffects.createEffect<unsigned long>(
        []() { return MachineState::current_state_ptr->getTimestamp(); },
        [](unsigned long timestamp) {
          apiServerBroadcastCallback.loop(timestamp);
        },
        false); // Update cadence too high to use as a message trigger (also it
                // would be weird as the trigger is to broadcast the message)
    apiEffects.onTriggered([]() { apiServerBroadcastCallback.reset(); });

    // Set up state broadcast callback
    apiServerBroadcastCallback.setCallback(
        []() { apiServer.broadcastState(stateUpdateMessage); });

    pConfig.setup();
    pConfig.onChange([](Config config) { apiServer.broadcastConfig(config); });

    WiFi.mode(WIFI_STA);
    WiFi.begin(SSID_NAME, SSID_PASWORD);

    // Set up OTA
    beginOTA(2040);

    // Start listening
    apiServer.onConnect([]() {
      apiServer.broadcastConfig(pConfig.getConfig());
      apiServer.broadcastState(stateUpdateMessage);
    });
    apiServer.begin();
  }
}

void loop() {
  // Run FSM & reconcile hardware with FSM state
  effects.loop();

  // Set API states
  apiEffects.loop();

  // Handle websocket API
  apiServer.loop();

  // Handle Arduino OTA
  handleOTA();
}
