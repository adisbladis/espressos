#pragma once

#include <cstddef>
#include <cstdint>

#include "../config.hpp"
#include "../fsm/fsmlist.hpp"
#include "../lib/result.hpp"
#include "../proto/api.h"

#define MSG_BUF_SIZE 128
#define UUID_SIZE 16 // Note: Convert this to bytes and get it down to 16 bytes
typedef Result<std::nullptr_t, const char *> APIResult;
typedef Command<UUID_SIZE> Cmd_t;

class APIHandler {
private:
  // Reuse command for every call to handle
  Cmd_t cmd;

  // Mutate persistent configuration
  PersistedConfig *pConfig;

  void handle(const uint8_t *requestID, PowerOn cmd) {
    PowerOnEvent powerOnEvent;
    powerOnEvent.setpoint = cmd.get_setpoint();
    send_event(powerOnEvent);
  };

  void handle(const uint8_t *requestID, PowerOff cmd) {
    send_event(PowerOffEvent());
  };

  void handle(const uint8_t *requestID, StartBrew cmd) {
    send_event(BrewStartEvent());
  };

  void handle(const uint8_t *requestID, StopBrew cmd) {
    send_event(BrewStopEvent());
  };

  void handle(const uint8_t *requestID, StartPump cmd) {
    send_event(StartPumpEvent());
  };

  void handle(const uint8_t *requestID, StopPump cmd) {
    send_event(StopPumpEvent());
  };

  void handle(const uint8_t *requestID, StartSteam cmd) {
    StartSteamEvent startSteamEvent;
    startSteamEvent.setpoint = cmd.get_setpoint();
    send_event(startSteamEvent);
  };

  void handle(const uint8_t *requestID, StopSteam cmd) {
    send_event(StopSteamEvent());
  };

  void handle(const uint8_t *requestID, BackflushStart cmd) {
    send_event(BackflushStartEvent());
  };

  void handle(const uint8_t *requestID, BackflushStop cmd) {
    send_event(BackflushStopEvent());
  };

  void handle(const uint8_t *requestID, RinseStart cmd) {
    send_event(RinseStartEvent());
  };

  void handle(const uint8_t *requestID, RinseStop cmd) {
    send_event(RinseStopEvent());
  };

  void handle(const uint8_t *requestID, BrewTarget cmd) {
    BrewTargetEvent brewTargetEvent;
    brewTargetEvent.value = cmd.get_value();

    switch (cmd.get_mode()) {
    case BrewTargetMode::POWER:
      brewTargetEvent.mode = BrewTargetEventMode::POWER;
      break;
    case BrewTargetMode::PRESSURE:
      brewTargetEvent.mode = BrewTargetEventMode::PRESSURE;
      break;
    }

    send_event(brewTargetEvent);
  };

  void handle(const uint8_t *requestID, Config config) {
    this->pConfig->setConfig(config);
  };

public:
  APIHandler(PersistedConfig *pConfig) : pConfig(pConfig){};

  APIResult handle(EmbeddedProto::ReadBufferFixedSize<MSG_BUF_SIZE> buf) {
    APIResult result;

    auto status = cmd.deserialize(buf);
    buf.clear();

    if (status != ::EmbeddedProto::Error::NO_ERRORS) {
      switch (status) {
      case ::EmbeddedProto::Error::END_OF_BUFFER:
        result.setError("error decoding command: While trying to read from the "
                        "buffer we ran out of bytes to read.");
        break;
      case ::EmbeddedProto::Error::BUFFER_FULL:
        result.setError("error decoding command: The write buffer is full, "
                        "unable to push more bytes in to it.");
        break;
      case ::EmbeddedProto::Error::INVALID_WIRETYPE:
        result.setError("error decoding command: When reading a Wiretype from "
                        "the tag we got an invalid value.");
        break;
      case ::EmbeddedProto::Error::ARRAY_FULL:
        result.setError("error decoding command: The array is full, it is not "
                        "possible to push more items in it.");
        break;
      case ::EmbeddedProto::Error::INVALID_FIELD_ID:
        result.setError("error decoding command: When the id "
                        "obtained from the tag equeals zero.");
        break;
      case ::EmbeddedProto::Error::OVERLONG_VARINT:
        result.setError("error decoding command: The maximum number of bytes "
                        "where read for this varint but we did not reach the "
                        "end of the data.");
        break;
      case ::EmbeddedProto::Error::INDEX_OUT_OF_BOUND:
        result.setError("error decoding command: You are trying to access an "
                        "index outside of valid data.");
        break;
      default:
        result.setError("error decoding command: unknown error");
        break;
      }

      return result;
    }

    auto requestID = cmd.request_id();

    switch (cmd.get_which_command_oneof()) {
    case Cmd_t::FieldNumber::NOT_SET:
      result.setError("oneof field not set");
      break;
    case Cmd_t::FieldNumber::POWER_ON:
      handle(requestID, cmd.get_power_on());
      break;
    case Cmd_t::FieldNumber::POWER_OFF:
      handle(requestID, cmd.get_power_off());
      break;
    case Cmd_t::FieldNumber::START_BREW:
      handle(requestID, cmd.get_start_brew());
      break;
    case Cmd_t::FieldNumber::STOP_BREW:
      handle(requestID, cmd.get_stop_brew());
      break;
    case Cmd_t::FieldNumber::START_PUMP:
      handle(requestID, cmd.get_start_pump());
      break;
    case Cmd_t::FieldNumber::STOP_PUMP:
      handle(requestID, cmd.get_stop_pump());
      break;
    case Cmd_t::FieldNumber::START_STEAM:
      handle(requestID, cmd.get_start_steam());
      break;
    case Cmd_t::FieldNumber::STOP_STEAM:
      handle(requestID, cmd.get_stop_steam());
      break;
    case Cmd_t::FieldNumber::CONFIG:
      handle(requestID, cmd.get_config());
      break;
    case Cmd_t::FieldNumber::BACKFLUSH_START:
      handle(requestID, cmd.get_backflush_start());
      break;
    case Cmd_t::FieldNumber::BACKFLUSH_STOP:
      handle(requestID, cmd.get_backflush_stop());
      break;
    case Cmd_t::FieldNumber::RINSE_START:
      handle(requestID, cmd.get_rinse_start());
      break;
    case Cmd_t::FieldNumber::RINSE_STOP:
      handle(requestID, cmd.get_rinse_stop());
    case Cmd_t::FieldNumber::BREW_TARGET_SET:
      handle(requestID, cmd.get_brew_target_set());
      break;
    default:
      result.setError("Logic error: Unhandled switch case");
      break;
    }

    cmd.clear();

    return result;
  };
};
