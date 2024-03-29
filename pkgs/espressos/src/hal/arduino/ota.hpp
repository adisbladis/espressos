#pragma once

#include <ArduinoOTA.h>

void beginArduinoOTA(int otaPort) {
  ArduinoOTA.setPort(otaPort);
  ArduinoOTA.begin();
}

void handleArduinoOTA() { ArduinoOTA.handle(); }
