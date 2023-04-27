#pragma once
#include "boiler.hpp"
#include "config.hpp"
#include "fsm/events.hpp"
#include "pressure.hpp"
#include "proto/api.h"
#include "websocket.hpp"
#include <ReadBufferFixedSize.h>
#include <WriteBufferFixedSize.h>

#include "config.hpp"
#include "fsm/fsmlist.hpp"
#include "interval_callback.hpp"
#include "logger.hpp"

#define MSG_BUF_SIZE 128
#define UUID_SIZE 16 // Note: Convert this to bytes and get it down to 16 bytes
#define ERROR_MESSAGE_SIZE 128

typedef Command<UUID_SIZE> Cmd_t;
typedef LogMessage<LOG_MESSAGE_SIZE> LogMessage_t;

class APIServer : public Logger {
private:
  WebServer server;
  PersistedConfig *pConfig;
  EmbeddedProto::ReadBufferFixedSize<MSG_BUF_SIZE> buf;
  EmbeddedProto::WriteBufferFixedSize<MSG_BUF_SIZE> outBuf;
  IntervalCallback stateUpdateCallback;
  StateUpdate<ERROR_MESSAGE_SIZE, ERROR_MESSAGE_SIZE> stateUpdateMessage;
  Cmd_t cmd;
  char logMessageBuf[LOG_MESSAGE_SIZE];

  // Preallocate a single event and re-use by setting oneof (also make sure to
  // reset requestId)
  Event<UUID_SIZE, ERROR_MESSAGE_SIZE, ERROR_MESSAGE_SIZE, LOG_MESSAGE_SIZE>
      event;

  bool hasClients() { return this->server.connectedClients() > 0; };

  // Broadcast the event singleton.
  //
  // This also implies relinquishing ownership of `event`.
  void broadcastEvent() {
    outBuf.clear();

    auto status = event.serialize(outBuf);
    event.clear();

    if (status != ::EmbeddedProto::Error::NO_ERRORS) {
      outBuf.clear();
      // Note: cannot use logger as it would cause infinite loops
      Serial.printf("encoding err: %d\n", status);
      return;
    }

    this->server.broadcastBIN(outBuf.get_data(), outBuf.get_size());
  }

  void broadcastConfig() {
    event.set_config(pConfig->getConfig());
    broadcastEvent();
  };

  void broadcastState() {
    auto machineState = MachineState::current_state_ptr;

    stateUpdateMessage.set_is_on(machineState->isOn());
    stateUpdateMessage.set_is_brewing(machineState->isBrewing());
    stateUpdateMessage.set_is_pumping(machineState->isPumping());
    stateUpdateMessage.set_is_steaming(machineState->isSteaming());

    event.clear_request_id();
    event.set_state_update(stateUpdateMessage);

    broadcastEvent();
  };

  void broadcastInitial() {
    broadcastState();
    broadcastConfig();
  };

public:
  APIServer(int port, PersistedConfig *pConfig)
      : server(port), stateUpdateCallback(STATE_UPDATE_INTERVAL),
        pConfig(pConfig) {

    stateUpdateCallback.setCallback([this]() {
      if (this->hasClients()) {
        this->broadcastState();
      }
    });

    pConfig->onChange([this](Config config) {
      if (this->hasClients()) {
        this->broadcastConfig();
      }
    });

    server.onEvent([this](uint8_t num, WStype_t type, uint8_t *payload,
                          size_t length) {
      switch (type) {
      case WStype_DISCONNECTED:
        Serial.printf("[%u] Disconnected!\n", num);
        break;

      case WStype_CONNECTED:
        broadcastInitial();
        break;

      case WStype_TEXT:
        // Emit a message in case of accidental text transmission during dev
        Serial.printf("[%u] received text: %s\n", num, payload);
        server.sendTXT(num, "text messages are not supported");
        break;

      case WStype_BIN:
        Serial.printf("[%u] received binary with length: %u\n", num, length);

        if (length > MSG_BUF_SIZE) {
          logger->log(LogLevel::ERROR, "length (%d) > MSG_BUF_SIZE (%d)",
                      length, MSG_BUF_SIZE);
          break;
        } else {
          memcpy(buf.get_data(), payload, length);
          buf.set_bytes_written(length);
        }

        auto status = cmd.deserialize(buf);
        buf.clear();

        if (status != ::EmbeddedProto::Error::NO_ERRORS) {
          switch (status) {
          case ::EmbeddedProto::Error::END_OF_BUFFER:
            logger->log(LogLevel::ERROR,
                        "error decoding command: While trying to read from the "
                        "buffer we ran out of bytes to read.");
            break;
          case ::EmbeddedProto::Error::BUFFER_FULL:
            logger->log(LogLevel::ERROR,
                        "error decoding command: The write buffer is full, "
                        "unable to push more bytes in to it.");
            break;
          case ::EmbeddedProto::Error::INVALID_WIRETYPE:
            logger->log(LogLevel::ERROR,
                        "error decoding command: When reading a Wiretype from "
                        "the tag we got an invalid value.");
            break;
          case ::EmbeddedProto::Error::ARRAY_FULL:
            logger->log(LogLevel::ERROR,
                        "error decoding command: The array is full, it is not "
                        "possible to push more items in it.");
            break;
          case ::EmbeddedProto::Error::INVALID_FIELD_ID:
            logger->log(LogLevel::ERROR, "error decoding command: When the id "
                                         "obtained from the tag equeals zero.");
            break;
          case ::EmbeddedProto::Error::OVERLONG_VARINT:
            logger->log(LogLevel::ERROR,
                        "error decoding command: The maximum number of bytes "
                        "where read for this varint but we did not reach the "
                        "end of the data.");
            break;
          case ::EmbeddedProto::Error::INDEX_OUT_OF_BOUND:
            logger->log(LogLevel::ERROR,
                        "error decoding command: You are trying to access an "
                        "index outside of valid data.");
            break;
          default:
            logger->log(LogLevel::ERROR,
                        "error decoding command: unknown error");
            break;
          }
          break;
        }

        logger->log(LogLevel::INFO, "Got request");

        switch (cmd.get_which_command_oneof()) {
        case Cmd_t::FieldNumber::NOT_SET:
          logger->log(LogLevel::ERROR, "oneof field not set");
          break;
        case Cmd_t::FieldNumber::POWER_ON:
          PowerOnEvent powerOnEvent;
          powerOnEvent.setpoint = cmd.get_power_on().get_setpoint();
          send_event(powerOnEvent);
          break;
        case Cmd_t::FieldNumber::POWER_OFF:
          send_event(PowerOffEvent());
          break;
        case Cmd_t::FieldNumber::START_BREW:
          send_event(StartBrewEvent());
          break;
        case Cmd_t::FieldNumber::STOP_BREW:
          send_event(StopBrewEvent());
          break;
        case Cmd_t::FieldNumber::START_PUMP:
          send_event(StartPumpEvent());
          break;
        case Cmd_t::FieldNumber::STOP_PUMP:
          send_event(StopPumpEvent());
          break;
        case Cmd_t::FieldNumber::START_STEAM:
          StartSteamEvent startSteamEvent;
          startSteamEvent.setpoint = cmd.get_start_steam().get_setpoint();
          send_event(startSteamEvent);
          break;
        case Cmd_t::FieldNumber::STOP_STEAM:
          send_event(StopSteamEvent());
        case Cmd_t::FieldNumber::CONFIG:
          this->pConfig->setConfig(cmd.get_config());
          break;
        case Cmd_t::FieldNumber::BACKFLUSH_START:
          send_event(BackflushStartEvent());
          break;
        case Cmd_t::FieldNumber::BACKFLUSH_STOP:
          send_event(BackflushStopEvent());
          break;
        default:
          logger->log(LogLevel::ERROR, "Logic error: Unhandled switch case");
          break;
        }

        cmd.clear();

        break;
      }
    });
  }

  void setSetpoint(int setpoint) {
    this->stateUpdateMessage.set_setpoint(setpoint);
  }

  void setBoilerTemp(TempReading temp) {
    auto boilerTempMsg = this->stateUpdateMessage.mutable_boilerTemp();

    if (temp.fault) {
      auto fs = boilerTempMsg.get_error();

      auto errorMessage = temp.errorMessage();
      fs.set(errorMessage, strlen(errorMessage));
      boilerTempMsg.set_error(fs);
    } else {
      boilerTempMsg.set_value(temp.temp);
    }

    this->stateUpdateMessage.set_boilerTemp(boilerTempMsg);
  }

  void log(LogLevel level, const char *message, ...) override {
    if (!this->hasClients()) {
      return;
    }

    va_list args;
    va_start(args, message);
    vsnprintf(logMessageBuf, LOG_MESSAGE_SIZE, message, args);
    va_end(args);

    auto log = event.get_log();

    switch (level) {
    case LogLevel::ERROR:
      log.set_logLevel(LogMessage_t::LogLevel::ERROR);
      break;
    case LogLevel::INFO:
      log.set_logLevel(LogMessage_t::LogLevel::INFO);
      break;
    case LogLevel::DEBUG:
      log.set_logLevel(LogMessage_t::LogLevel::DEBUG);
      break;
    default:
      log.set_logLevel(LogMessage_t::LogLevel::INFO);
      break;
    }

    auto fs = log.get_msg();
    fs.set(logMessageBuf, strlen(logMessageBuf) > LOG_MESSAGE_SIZE
                              ? LOG_MESSAGE_SIZE
                              : strlen(logMessageBuf));
    log.set_msg(fs);

    event.set_log(log);
    broadcastEvent();
  }

  void setPressure(PressureSensorResult_t pressure) {
    auto pressureMsg = this->stateUpdateMessage.mutable_pressure();

    if (pressure.hasError()) {
      auto fs = pressureMsg.get_error();
      fs.set("Value out of bounds", 19);
      pressureMsg.set_error(fs);
    } else {
      pressureMsg.set_value(pressure.getValue());
    }

    this->stateUpdateMessage.set_pressure(pressureMsg);
  }

  void setStateUpdateInterval(long interval) {
    this->stateUpdateCallback.setInterval(interval);
  }

  void loop() {
    this->server.loop();
    this->stateUpdateCallback.loop();
  }

  void begin() { this->server.begin(); }

  void close() { this->server.close(); }
};

// A logger that emits log messages to websockets
class APIServerLogger : public Logger {
private:
  APIServer apiServer;

public:
  APIServerLogger(APIServer apiServer) : apiServer(apiServer){};
};
