#pragma once

#include "../../logger.hpp"
#include <ArduinoOTA.h>

void beginArduinoOTA(int otaPort) {
  ArduinoOTA.setPort(2040);

  ArduinoOTA.onStart([]() { logger->log(LogLevel::DEBUG, "OTA start"); });
  ArduinoOTA.onEnd([]() { logger->log(LogLevel::DEBUG, "OTA end"); });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    // logger->log(LogLevel::DEBUG, "OTA progress: %u%%", pct);
  });
  ArduinoOTA.onError([](ota_error_t error) {
    const char *message = "Unknown error";

    switch (error) {
    case OTA_AUTH_ERROR:
      message = "Auth Failed";
    case OTA_BEGIN_ERROR:
      message = "Begin Failed";
    case OTA_CONNECT_ERROR:
      message = "Connect Failed";
    case OTA_RECEIVE_ERROR:
      message = "Receive Failed";
    case OTA_END_ERROR:
      message = "End Failed";
    }

    logger->log(LogLevel::ERROR, "OTA Error[%u]: %s", error, message);
  });

  ArduinoOTA.begin();
}

void handleArduinoOTA() { ArduinoOTA.handle(); }
