#include <ArduinoOTA.h>
#include <LittleFS.h>
#include <PIDController.hpp>
#include <WiFi.h>
#include <cstdint>
#include <dimmable_light.h>

#include "api/handler.hpp"
#include "cachedpin.hpp"
#include "config.hpp"
#include "fsm/brew.hpp"
#include "fsm/events.hpp"
#include "fsm/fsmlist.hpp"
#include "fsm/machine.hpp"
#include "fsm/pump.hpp"
#include "http/api.hpp"
#include "lib/effects.hpp"
#include "lib/map.hpp"
#include "lib/timers.hpp"
#include "logger.hpp"
#include "ota.hpp"
#include "proto/api.h"
#include "hal/setup.hpp"

// Hardware IO
static DimmableLight pump(PUMP_DIMMER_OUT);

static PersistedConfig pConfig;
static APIHandler apiHandler(&pConfig);
static APIWebServer apiServer = APIWebServer(HTTP_PORT, &apiHandler);

// Watch variables for change and propagate to hardware/API
static Effects effects;
static Effects apiEffects;
static Timers timers;
static Timers apiTimers;

// Re-use loop event on every iteration
static LoopEvent loopEvent;
static TimeEvent timeEvent;

static StateUpdate<ERROR_MESSAGE_SIZE, ERROR_MESSAGE_SIZE> stateUpdateMessage;

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

  setupHAL(timers);

  // Run FSM loops on millisecond change
  effects.createEffect<unsigned long>(millis, [](unsigned long now) {
    timeEvent.timestamp = now;
    send_event(timeEvent);

    loopEvent.timestamp = now;
    send_event(loopEvent);
  });

  // Run boiler PID
  {
    static constexpr int WindowSize = 100;
    static PIDController<int32_t, float, unsigned long> boilerPID(
        0, BOILER_PID_P, BOILER_PID_I, BOILER_PID_D, REVERSE);
    boilerPID.Begin(AUTOMATIC, millis());
    static unsigned long windowStartTime = millis();

    effects.createEffect<std::uint16_t>(
        []() { return MachineState::current_state_ptr->getSetPoint(); },
        [](std::uint16_t setpoint) { boilerPID.SetSetpoint(setpoint); });

    static CachedOutputPin outputPin(BOILER_SSR_PIN);
    outputPin.setup();

    timers.createInterval(1, []() {
      unsigned long now = MachineState::current_state_ptr->getTimestamp();
      std::int16_t temp = MachineState::current_state_ptr->getTemp();
      std::int32_t Output;

      boilerPID.Compute(now, temp, &Output);

      if (now - windowStartTime >= WindowSize) {
        windowStartTime += WindowSize;
      }

      if (Output < now - windowStartTime) {
        outputPin.digitalWrite(HIGH);
      } else {
        outputPin.digitalWrite(LOW);
      }
    });
  }

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

    // React to state machine pump changes
    effects.createEffect<PumpTarget>(
        []() { return MachineState::current_state_ptr->getPump(); },
        [](PumpTarget pumpTarget) {
          switch (pumpTarget.mode) {
          case PumpMode::POWER:
            // Remap value into uint8 range used by dimmer and apply
            pumpPower = map<uint16_t>(pumpTarget.value, 0, 65535, 0, 255);
            break;

          case PumpMode::PRESSURE:
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
    apiEffects.createEffect<std::int16_t>(
        []() { return MachineState::current_state_ptr->getTemp(); },
        [](std::int16_t temp) {
          auto boilerTempMsg = stateUpdateMessage.mutable_boilerTemp();
          boilerTempMsg.set_value(temp);
          stateUpdateMessage.set_boilerTemp(boilerTempMsg);
        },
        false); // Update cadence too high to use as update trigger

    // Set pressure in API server
    apiEffects.createEffect<std::uint16_t>(
        []() { return MachineState::current_state_ptr->getPressure(); },
        [](std::uint16_t pressure) {
          auto pressureMsg = stateUpdateMessage.mutable_pressure();
          pressureMsg.set_value(pressure);
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
    static auto stateUpdateTimer =
        apiTimers.createInterval(STATE_UPDATE_INTERVAL, []() {
          apiServer.broadcastState(stateUpdateMessage);
        });

    apiEffects.createEffect<unsigned long>(
        []() {
          return MachineState::current_state_ptr->getStateUpdateInterval();
        },
        [](unsigned long updateInterval) {
          stateUpdateTimer->interval = updateInterval;
        },
        false); // The update interval is irrelevant to the API server

    // Set current brew target
    apiEffects.createEffect<BrewStateTarget>(
        []() { return BrewState::current_state_ptr->getTarget(); },
        [](BrewStateTarget target) {
          auto brewTarget = stateUpdateMessage.mutable_brew_target();
          brewTarget.set_value(target.value);

          switch (target.mode) {
          case BrewStateMode::POWER:
            brewTarget.set_mode(BrewTargetMode::POWER);
            break;
          case BrewStateMode::PRESSURE:
            brewTarget.set_mode(BrewTargetMode::PRESSURE);
            break;
          }

          stateUpdateMessage.set_brew_target(brewTarget);
        },
        false); // Update cadence too high to use as a message trigger

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
    apiEffects.onTriggered([]() { stateUpdateTimer->last = 0; });

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

  {
    unsigned long now = MachineState::current_state_ptr->getTimestamp();

    timers.loop(now);

    apiTimers.loop(now);
  }

  // Handle websocket API
  apiServer.loop();

  // Handle Arduino OTA
  handleOTA();
}
