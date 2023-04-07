#pragma once
#include "proto/api.h"
#include "websocket.hpp"
#include <ReadBufferFixedSize.h>

// 1024 is an arbitrary buffer size
#define MSG_BUF_SIZE 128
#define UUID_SIZE 36

class APIServer {
private:
  WebServer server;

public:
  APIServer(int port) : server(port) {
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

        EmbeddedProto::ReadBufferFixedSize<MSG_BUF_SIZE> buf;

        if (length > MSG_BUF_SIZE) {
          Serial.println("length > MSG_BUF_SIZE");
          // TODO: Send error message
          break;
        } else {
          memcpy(buf.get_data(), payload, length);
          buf.set_bytes_written(length);
        }

        // Client sends UUID4 as id which is 36 bytes in length
        Command<UUID_SIZE> cmd;
        auto status = cmd.deserialize(buf);
        if (status != ::EmbeddedProto::Error::NO_ERRORS) {
          Serial.printf("err status: %d\n", status);
          // TODO: Send error message
          break;
        }

        // switch (cmd.get_which_command_oneof()) {
        // case FieldNumber::POWER_ON:
        //   // set_power_on(rhs.get_power_on());
        //   break;

        // case FieldNumber::POWER_OFF:
        //   // set_power_off(rhs.get_power_off());
        //   break;

        // case FieldNumber::START_BREW:
        //   // set_start_brew(rhs.get_start_brew());
        //   break;

        // case FieldNumber::STOP_BREW:
        //   // set_stop_brew(rhs.get_stop_brew());
        //   break;

        // case FieldNumber::START_STEAM:
        //   // set_start_steam(rhs.get_start_steam());
        //   break;

        // case FieldNumber::STOP_STEAM:
        //   // set_stop_steam(rhs.get_stop_steam());
        //   break;

        // case FieldNumber::SET_STEAM_SET_POINT:
        //   // set_set_steam_set_point(rhs.get_set_steam_set_point());
        //   break;

        // case FieldNumber::SET_BOILER_SET_POINT:
        //   // set_set_boiler_set_point(rhs.get_set_boiler_set_point());
        //   break;

        // default:
        //   break;
        // }

        // send message to client
        server.sendBIN(num, payload, length);
        break;
      }
    });
  }

  void loop() { this->server.loop(); }

  void begin() { this->server.begin(); }

  void close() { this->server.close(); }

};
