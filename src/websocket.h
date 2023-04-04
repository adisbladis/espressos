#pragma once

#include <WebSocketsServer.h>

class WebServer : public WebSocketsServer {
public:
  WebServer(int port) : WebSocketsServer(port) {}

  void handleNonWebsocketConnection(WSclient_t *client) override {
    // TODO: Serve up web interface
    Serial.println("Got non websocket connection");
    clientDisconnect(client);
  }
};
