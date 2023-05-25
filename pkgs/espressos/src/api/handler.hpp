#pragma once

#include <ReadBufferFixedSize.h>
#include <cstddef>
#include <cstdint>

#include "../fsm/fsmlist.hpp"
#include "../proto/api.h"

constexpr int MsgBufSize = 128;
constexpr int UUIDSize = 16;

typedef Command<UUIDSize> Cmd_t;

class APIHandler {
private:
  // Reuse command for every call to handle
  Cmd_t cmd;

  static void handle(const uint8_t *requestID, const PowerOn &cmd) {
    send_event(PowerOnEvent());
  };

  static void handle(const uint8_t *requestID, const PowerOff &cmd) {
    send_event(PowerOffEvent());
  };

  static void handle(const uint8_t *requestID, const StartBrew &cmd) {
    send_event(BrewStartEvent());
  };

  static void handle(const uint8_t *requestID, const StopBrew &cmd) {
    send_event(BrewStopEvent());
  };

  static void handle(const uint8_t *requestID, const StartPump &cmd) {
    send_event(PumpStartEvent());
  };

  static void handle(const uint8_t *requestID, const StopPump &cmd) {
    send_event(PumpStopEvent());
  };

  static void handle(const uint8_t *requestID, const StartSteam &cmd) {
    send_event(SteamStartEvent());
  };

  static void handle(const uint8_t *requestID, const StopSteam &cmd) {
    send_event(SteamStopEvent());
  };

  static void handle(const uint8_t *requestID, const BackflushStart &cmd) {
    send_event(BackflushStartEvent());
  };

  static void handle(const uint8_t *requestID, const BackflushStop &cmd) {
    send_event(BackflushStopEvent());
  };

  static void handle(const uint8_t *requestID, const RinseStart &cmd) {
    send_event(RinseStartEvent());
  };

  static void handle(const uint8_t *requestID, const RinseStop &cmd) {
    send_event(RinseStopEvent());
  };

  static void handle(const uint8_t *requestID, const BrewTarget &cmd) {
    BrewTargetEvent brewTargetEvent();
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

  static void handle(const uint8_t *requestID, const Config &config) {
    send_event(ConfigSetEvent(config));
  };

  static void handle(const uint8_t *requestID, const SetpointSet &cmd) {
    send_event(SetpointSetEvent(cmd.get_setpoint()));
  };

public:
  APIHandler(){};

  const char *handle( // NOLINT(readability-convert-member-functions-to-static)
      EmbeddedProto::ReadBufferFixedSize<MsgBufSize> &buf) {
    const char *error = nullptr;

    auto status = cmd.deserialize(buf);
    buf.clear();

    if (status != ::EmbeddedProto::Error::NO_ERRORS) {
      switch (status) {
      case ::EmbeddedProto::Error::END_OF_BUFFER:
        error = "error decoding command: While trying to read from the "
                "buffer we ran out of bytes to read.";
        break;
      case ::EmbeddedProto::Error::BUFFER_FULL:
        error = "error decoding command: The write buffer is full, "
                "unable to push more bytes in to it.";
        break;
      case ::EmbeddedProto::Error::INVALID_WIRETYPE:
        error = "error decoding command: When reading a Wiretype from "
                "the tag we got an invalid value.";
        break;
      case ::EmbeddedProto::Error::ARRAY_FULL:
        error = "error decoding command: The array is full, it is not "
                "possible to push more items in it.";
        break;
      case ::EmbeddedProto::Error::INVALID_FIELD_ID:
        error = "error decoding command: When the id "
                "obtained from the tag equeals zero.";
        break;
      case ::EmbeddedProto::Error::OVERLONG_VARINT:
        error = "error decoding command: The maximum number of bytes "
                "where read for this varint but we did not reach the "
                "end of the data.";
        break;
      case ::EmbeddedProto::Error::INDEX_OUT_OF_BOUND:
        error = "error decoding command: You are trying to access an "
                "index outside of valid data.";
        break;
      default:
        error = "error decoding command: unknown error";
        break;
      }

      cmd.clear();

      return error;
    }

    auto requestID = cmd.request_id();

    switch (cmd.get_which_command_oneof()) {
    case Cmd_t::FieldNumber::NOT_SET:
      error = "oneof field not set";
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
      break;
    case Cmd_t::FieldNumber::BREW_TARGET_SET:
      handle(requestID, cmd.get_brew_target_set());
      break;
    case Cmd_t::FieldNumber::SETPOINT_SET:
      handle(requestID, cmd.get_setpoint_set());
      break;
    default:
      error = "Logic error: Unhandled switch case";
      break;
    }

    cmd.clear();

    return error;
  };
};
