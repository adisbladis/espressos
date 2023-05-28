#pragma once

#include <ReadBufferFixedSize.h>
#include <WebSocketsServer.h>
#include <WriteBufferFixedSize.h>

#include <api.h>
#include <config.h>
#include <cstdint>
#include <fsmlist.hpp>

#include "../../api/handler.hpp"
#include "../../logger.hpp"

typedef LogMessage<LOG_MESSAGE_SIZE> LogMessage_t;

class WebServer : public WebSocketsServer {
public:
  explicit WebServer(int port) : WebSocketsServer(port) {
    this->_server->setNoDelay(true);
  }

  void handleNonWebsocketConnection(WSclient_t *client) override {
    // TODO: Serve up web interface
    Serial.println("Got non websocket connection");
    clientDisconnect(client);
  }
};

class APIWebServer : public Logger {
private:
  WebServer server;
  APIHandler handler;
  EmbeddedProto::ReadBufferFixedSize<MsgBufSize> buf;
  EmbeddedProto::WriteBufferFixedSize<MsgBufSize> outBuf;
  char logMessageBuf[LOG_MESSAGE_SIZE];
  std::function<void()> onConnectCallback;

  // Preallocate a single event and re-use by setting oneof (also make sure to
  // reset requestId)
  Event<ERROR_MESSAGE_SIZE, LOG_MESSAGE_SIZE, UUIDSize, ERROR_MESSAGE_SIZE>
      event;

  inline bool hasClients() { return this->server.connectedClients() > 0; };

  // Broadcast the event singleton.
  //
  // This also implies relinquishing ownership of `event` as it's cleared.
  //
  // NOLINTBEGIN(readability-convert-member-functions-to-static)
  void broadcastEvent() {
    outBuf.clear();

    if (!this->hasClients()) {
      event.clear();
      return;
    }

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
  // NOLINTEND(readability-convert-member-functions-to-static)

  // Send the event singleton to a single socket.
  //
  // This also implies relinquishing ownership of `event` as it's cleared.
  //
  // NOLINTBEGIN(readability-convert-member-functions-to-static)
  void sendEvent(uint8_t num) {
    outBuf.clear();

    auto status = event.serialize(outBuf);
    event.clear();

    if (status != ::EmbeddedProto::Error::NO_ERRORS) {
      outBuf.clear();
      // Note: cannot use logger as it would cause infinite loops
      Serial.printf("encoding err: %d\n", status);
      return;
    }

    this->server.sendBIN(num, outBuf.get_data(), outBuf.get_size());
  }
  // NOLINTEND(readability-convert-member-functions-to-static)

public:
  explicit APIWebServer(int port)
      : server(port), handler(), onConnectCallback([]() {}) {

    server.onEvent([this](uint8_t num, WStype_t type, uint8_t *payload,
                          size_t length) {
      switch (type) {
      case WStype_DISCONNECTED:
        Serial.printf("[%u] Disconnected!\n", num);
        break;

      case WStype_CONNECTED:
        this->onConnectCallback();
        break;

      case WStype_TEXT:
        // Emit a message in case of accidental text transmission during dev
        Serial.printf("[%u] received text: %s\n", num, payload);
        server.sendTXT(num, "text messages are not supported");
        break;

      case WStype_BIN:
        Serial.printf("[%u] received binary with length: %u\n", num, length);

        logger->log(LogLevel::INFO, "Got request");
        if (length > MsgBufSize) {
          logger->log(LogLevel::ERROR, "length (%d) > MsgBufSize (%d)", length,
                      MsgBufSize);
          break;
        } else {
          memcpy(buf.get_data(), payload, length);
          buf.set_bytes_written(length);
        }

        // Dispatch to APIHandler
        auto resp = event.mutable_resp();
        this->handler.handle(buf, resp);
        buf.clear();

        // Send response
        event.set_resp(resp);
        sendEvent(num);

        break;
      }
    });
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

    // NOLINTBEGIN(bugprone-branch-clone)
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
    // NOLINTEND(bugprone-branch-clone)

    auto fieldString = log.get_msg();
    fieldString.set(logMessageBuf, strlen(logMessageBuf) > LOG_MESSAGE_SIZE
                                       ? LOG_MESSAGE_SIZE
                                       : strlen(logMessageBuf));
    log.set_msg(fieldString);

    event.set_log(log);
    broadcastEvent();
  }

  void loop() { this->server.loop(); }

  void begin() { this->server.begin(); }

  void close() { this->server.close(); }

  void
  broadcastState(const StateUpdate<ERROR_MESSAGE_SIZE> &stateUpdateMessage) {
    if (!this->hasClients()) {
      return;
    }

    event.set_state_update(stateUpdateMessage);
    broadcastEvent();
  };

  void broadcastConfig(const Config &config) {
    if (!this->hasClients()) {
      return;
    }

    event.set_config(config);
    broadcastEvent();
  };

  void onConnect(std::function<void()> callback) {
    onConnectCallback = callback;
  };
};
