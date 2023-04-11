#pragma once
#include "boiler.hpp"
#include "config.hpp"
#include "proto/api.h"
#include "websocket.hpp"
#include <ReadBufferFixedSize.h>
#include <WriteBufferFixedSize.h>

#include "config.hpp"
#include "fsm/fsmlist.hpp"
#include "interval_callback.hpp"

#define MSG_BUF_SIZE 128
#define UUID_SIZE 16 // Note: Convert this to bytes and get it down to 16 bytes
#define ERROR_MESSAGE_SIZE 128

typedef Command<UUID_SIZE> Cmd_t;

class APIServer {
private:
  WebServer server;
  PersistedConfig *pConfig;
  EmbeddedProto::ReadBufferFixedSize<MSG_BUF_SIZE> buf;
  EmbeddedProto::WriteBufferFixedSize<MSG_BUF_SIZE> outBuf;
  IntervalCallback stateUpdateCallback;
  StateUpdate<ERROR_MESSAGE_SIZE, ERROR_MESSAGE_SIZE> stateUpdateMessage;
  Cmd_t cmd;

  // Preallocate a single event and re-use by setting oneof (also make sure to
  // reset requestId)
  Event<UUID_SIZE, ERROR_MESSAGE_SIZE, ERROR_MESSAGE_SIZE> event;

  // Broadcast the event singleton.
  //
  // This also implies relinquishing ownership of `event`.
  void broadcastEvent() {
    outBuf.clear();

    auto status = event.serialize(outBuf);
    event.clear();

    if (status != ::EmbeddedProto::Error::NO_ERRORS) {
      outBuf.clear();
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
    stateUpdateMessage.set_is_on(!(MachineState::is_in_state<Off>() ||
                                   MachineState::is_in_state<Panic>()));
    stateUpdateMessage.set_is_brewing(MachineState::is_in_state<Brewing>());
    stateUpdateMessage.set_is_pumping(MachineState::is_in_state<Pumping>());
    stateUpdateMessage.set_is_steaming(MachineState::is_in_state<Steaming>());

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

    stateUpdateCallback.setCallback([this]() { this->broadcastState(); });

    pConfig->onChange([this](Config config) { this->broadcastConfig(); });

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
          Serial.println("length > MSG_BUF_SIZE");
          // TODO: Send error message
          break;
        } else {
          memcpy(buf.get_data(), payload, length);
          buf.set_bytes_written(length);
        }

        auto status = cmd.deserialize(buf);
        buf.clear();

        if (status != ::EmbeddedProto::Error::NO_ERRORS) {
          Serial.printf("err status: %d\n", status);
          // TODO: Send error message
          break;
        }

        // TODO: Request logging?

        // Config newConfig = cmd.get_config();

        switch (cmd.get_which_command_oneof()) {
        case Cmd_t::FieldNumber::NOT_SET:
          // TODO: Error resp
          Serial.println("oneof: Not set");
          break;
        case Cmd_t::FieldNumber::POWER_ON:
          send_event(PowerOnEvent());
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
          send_event(StartSteamEvent());
          break;
        case Cmd_t::FieldNumber::STOP_STEAM:
          send_event(StopSteamEvent());
        case Cmd_t::FieldNumber::CONFIG:
          this->pConfig->setConfig(cmd.get_config());
          break;
        default:
          // TODO: Error resp
          Serial.println("Logic error: Unhandled switch case");
          break;
        }

        cmd.clear();

        break;
      }
    });
  }

  void setBoilerTemp(TempReading temp) {
    auto boilerTempMsg = this->stateUpdateMessage.mutable_boilerTemp();

    if (temp.fault) {
      // TODO: Emit error message
      // TODO: Enter panic state
      // boilerTempMsg.set_error(temp.error);
    } else {
      boilerTempMsg.set_value(temp.temp);
    }

    this->stateUpdateMessage.set_boilerTemp(boilerTempMsg);
  }

  void setPressure(float pressure) {
    auto pressureMsg = this->stateUpdateMessage.mutable_pressure();
    pressureMsg.set_value(pressure);
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
