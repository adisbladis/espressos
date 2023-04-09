#pragma once
#include "boiler.hpp"
#include "proto/api.h"
#include "websocket.hpp"
#include <ReadBufferFixedSize.h>
#include <WriteBufferFixedSize.h>

#include "fsm/fsmlist.hpp"
#include "interval_callback.hpp"

#define MSG_BUF_SIZE 128
#define UUID_SIZE 16 // Note: Convert this to bytes and get it down to 16 bytes
#define ERROR_MESSAGE_SIZE 128

typedef Command<UUID_SIZE> Cmd_t;
typedef StateUpdate<ERROR_MESSAGE_SIZE, ERROR_MESSAGE_SIZE> StateUpdate_t;

class APIServer {
private:
  WebServer server;
  EmbeddedProto::ReadBufferFixedSize<MSG_BUF_SIZE> buf;
  EmbeddedProto::WriteBufferFixedSize<MSG_BUF_SIZE> stateUpdateMessageBuf;
  IntervalCallback stateUpdateCallback;

public:
  StateUpdate_t stateUpdateMessage; // TODO: Make private

  APIServer(int port)
      : server(port), stateUpdateCallback(STATE_UPDATE_INTERVAL) {

    stateUpdateCallback.setCallback([this]() {
      stateUpdateMessage.set_is_on(!(MachineState::is_in_state<Off>() ||
                                     MachineState::is_in_state<Panic>()));
      stateUpdateMessage.set_is_brewing(MachineState::is_in_state<Brewing>());
      stateUpdateMessage.set_is_pumping(MachineState::is_in_state<Pumping>());
      stateUpdateMessage.set_is_steaming(MachineState::is_in_state<Steaming>());

      Event<UUID_SIZE, ERROR_MESSAGE_SIZE, ERROR_MESSAGE_SIZE> serverEvent;
      serverEvent.set_state_update(stateUpdateMessage);

      auto status = serverEvent.serialize(stateUpdateMessageBuf);
      if (status != ::EmbeddedProto::Error::NO_ERRORS) {
        stateUpdateMessageBuf.clear();
        Serial.printf("err status: %d\n", status);
        return;
      }
      // Serial.println(stateUpdateMessage.get_is_on());
      // Serial.println(stateUpdateMessage.get_boilerTemp().get_value());

      auto res = this->server.broadcastBIN(stateUpdateMessageBuf.get_data(),
                                           stateUpdateMessageBuf.get_size());

      stateUpdateMessageBuf.clear();
    });

    server.onEvent([this](uint8_t num, WStype_t type, uint8_t *payload,
                          size_t length) {
      switch (type) {
      case WStype_DISCONNECTED:
        Serial.printf("[%u] Disconnected!\n", num);
        break;
      case WStype_CONNECTED: {
        // TODO: Send initial state message
      } break;
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

        Cmd_t cmd;
        auto status = cmd.deserialize(buf);
        if (status != ::EmbeddedProto::Error::NO_ERRORS) {
          Serial.printf("err status: %d\n", status);
          // TODO: Send error message
          break;
        }

        // TODO: Request logging?

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
          break;
        default:
          // TODO: Error resp
          Serial.println("Logic error: Unhandled switch case");
          break;
        }

        buf.clear();
        break;
      }
    });
  }

  // TODO: Mutable
  void setBoilerTemp(TempReading temp) {
    auto boilerTempMsg = this->stateUpdateMessage.get_boilerTemp();

    if (temp.fault) {
      // TODO: Emit error message
      // TODO: Enter panic state
      // boilerTempMsg.set_error(temp.error);
    } else {
      boilerTempMsg.set_value(temp.temp);
    }

    this->stateUpdateMessage.set_boilerTemp(boilerTempMsg);
  }

  // TODO: Mutable
  void setPressure(float pressure) {
    // TODO: Handle errors (probably introduce a wrapping class/struct)
    auto pressureMsg = this->stateUpdateMessage.get_pressure();
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
