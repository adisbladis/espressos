#pragma once

#include <LittleFS.h>
#include <ReadBufferFixedSize.h>
#include <WriteBufferFixedSize.h>

#include "logger.hpp"
#include "proto/config.h"

#define CONFIG_FILE "config.pb"
#define CONFIG_BUF_SIZE 128

class PersistedConfig {
private:
  Config config;
  std::vector<std::function<void(Config)>> callbacks;

  // Set initial default values
  void initialise() {
    config = Config();

    auto boiler = config.mutable_boiler();
    boiler.set_rref(BOILER_RREF);
    boiler.set_rnominal(BOILER_RNOMINAL);

    auto pid = boiler.mutable_PID();
    pid.set_P(BOILER_PID_P);
    pid.set_I(BOILER_PID_I);
    pid.set_D(BOILER_PID_D);
    boiler.set_PID(pid);

    config.set_boiler(boiler);
    config.set_setpoint(BOILER_SETPOINT);
    config.set_steamSetPoint(BOILER_STEAM_SETPOINT);
  }

public:
  bool save() {
    EmbeddedProto::WriteBufferFixedSize<CONFIG_BUF_SIZE> buf;
    auto status = config.serialize(buf);
    if (status != ::EmbeddedProto::Error::NO_ERRORS) {
      logger->log(LogLevel::ERROR, "Error saving config: %d\n", status);
    }

    File f = LittleFS.open(CONFIG_FILE, "w");
    if (!f) {
      logger->log(LogLevel::ERROR, "Could not open config file for writing");
      return false;
    }

    auto n = f.write(buf.get_data(), buf.get_size());
    f.close();

    if (n != buf.get_size()) {
      logger->log(LogLevel::ERROR, "Error writing config file");
      return false;
    }

    return true;
  }

  Config getConfig() { return config; };

  bool setConfig(const Config &c) {
    config = c;

    for (auto cb : callbacks) {
      cb(config);
    }

    return save();
  };

  void onChange(std::function<void(Config)> cb) { callbacks.push_back(cb); };

  void setup() {
    File f = LittleFS.open(CONFIG_FILE, "r");
    if (f) {
      EmbeddedProto::ReadBufferFixedSize<CONFIG_BUF_SIZE> buf;
      f.read(buf.get_data(), CONFIG_BUF_SIZE);
      buf.set_bytes_written(f.size());
      auto status = config.deserialize(buf);
      if (status != ::EmbeddedProto::Error::NO_ERRORS) {
        initialise();
        logger->log(LogLevel::ERROR, "Error decoding config: %d\n", status);
        return;
      }
    } else {
      initialise();
      save();
    }
  }
};
