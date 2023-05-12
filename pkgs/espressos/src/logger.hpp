#pragma once

#include <stdarg.h>
#include <stdio.h>

#define LOG_MESSAGE_SIZE 128

enum LogLevel { ERROR, INFO, DEBUG };

// A logger that discards all messages
class Logger {
public:
  virtual void log(LogLevel level, const char *message, ...){};
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

static Logger *logger = new Logger();

static void setLogger(Logger *l) { logger = l; }
