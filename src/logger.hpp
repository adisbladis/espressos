#pragma once

#include <Arduino.h>
#include <stdarg.h>
#include <stdio.h>

#define LOG_MESSAGE_SIZE 128

enum LogLevel { ERROR, INFO, DEBUG };

const char *logLevelName(LogLevel level) {
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

// A logger that discards all messages
class Logger {
public:
  virtual void log(LogLevel level, const char *message, ...){};
};

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

// MultiLogger binds together multiple loggers
class MultiLogger : public Logger {
private:
  std::vector<Logger *> loggers;

public:
  void log(LogLevel level, const char *message, ...) override {
    va_list args;
    va_start(args, message);

    for (auto logger : loggers) {
      logger->log(level, message, args);
    }

    va_end(args);
  }

  void add(Logger *logger) { loggers.push_back(logger); }
};

Logger *logger = new Logger();

void setLogger(Logger *l) { logger = l; }
