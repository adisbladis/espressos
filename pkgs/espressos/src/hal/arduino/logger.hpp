#pragma once

#include <Arduino.h>
#include <stdarg.h>
#include <stdio.h>

#include "../../logger.hpp"

static const char *logLevelName(LogLevel level) {
  switch (level) {
  case LogLevel::ERROR:
    return "ERROR";
  case LogLevel::INFO:
    return "INFO";
  case LogLevel::DEBUG:
    return "DEBUG";
  default:
    return String(level).c_str();
  }
}

// A logger that prints messages on the serial output
class SerialLogger : public Logger {
private:
  char buf[LOG_MESSAGE_SIZE];

public:
  void log(LogLevel level, const char *message, ...) override {
    va_list args;
    va_start(args, message);
    vsnprintf(buf, LOG_MESSAGE_SIZE, message, args);
    va_end(args);

    Serial.print("log.");
    Serial.print(logLevelName(level));
    Serial.print(": ");
    Serial.print(buf);
    Serial.print("\n");
  }
};
