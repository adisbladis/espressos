#pragma once

#include <ReadBufferFixedSize.h>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <iostream>
#include <optional>
#include <string>

#include <api.h>
#include <fsmlist.hpp>

constexpr int MsgBufSize = 128;
constexpr int UUIDSize = 16;

typedef Command<UUIDSize> Cmd_t;

class APIHandler {
private:
  // Reuse command for every call to handle
  Cmd_t cmd;

  static void handle(const PowerOn &cmd) { send_event(PowerOnEvent()); };

  static void handle(const PowerOff &cmd) { send_event(PowerOffEvent()); };

  static void handle(const StartBrew &cmd) { send_event(BrewStartEvent()); };

  static void handle(const StopBrew &cmd) { send_event(BrewStopEvent()); };

  static void handle(const StartPump &cmd) { send_event(PumpStartEvent()); };

  static void handle(const StopPump &cmd) { send_event(PumpStopEvent()); };

  static void handle(const StartSteam &cmd) { send_event(SteamStartEvent()); };

  static void handle(const StopSteam &cmd) { send_event(SteamStopEvent()); };

  static void handle(const BackflushStart &cmd) {
    send_event(BackflushStartEvent());
  };

  static void handle(const BackflushStop &cmd) {
    send_event(BackflushStopEvent());
  };

  static void handle(const RinseStart &cmd) { send_event(RinseStartEvent()); };

  static void handle(const RinseStop &cmd) { send_event(RinseStopEvent()); };

  static void handle(const BrewTarget &cmd) {
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

  static void handle(const Config &config) {
    send_event(ConfigSetEvent(config));
  };

  static void handle(const SetpointSet &cmd) {
    send_event(SetpointSetEvent(cmd.get_setpoint()));
  };

public:
  APIHandler(){};

  std::optional<std::string> handle(const Cmd_t &cmd) {
    switch (cmd.get_which_command_oneof()) {
    case Cmd_t::FieldNumber::NOT_SET:
      return "oneof field not set";
    case Cmd_t::FieldNumber::POWER_ON:
      handle(cmd.get_power_on());
      break;
    case Cmd_t::FieldNumber::POWER_OFF:
      handle(cmd.get_power_off());
      break;
    case Cmd_t::FieldNumber::START_BREW:
      handle(cmd.get_start_brew());
      break;
    case Cmd_t::FieldNumber::STOP_BREW:
      handle(cmd.get_stop_brew());
      break;
    case Cmd_t::FieldNumber::START_PUMP:
      handle(cmd.get_start_pump());
      break;
    case Cmd_t::FieldNumber::STOP_PUMP:
      handle(cmd.get_stop_pump());
      break;
    case Cmd_t::FieldNumber::START_STEAM:
      handle(cmd.get_start_steam());
      break;
    case Cmd_t::FieldNumber::STOP_STEAM:
      handle(cmd.get_stop_steam());
      break;
    case Cmd_t::FieldNumber::CONFIG:
      handle(cmd.get_config());
      break;
    case Cmd_t::FieldNumber::BACKFLUSH_START:
      handle(cmd.get_backflush_start());
      break;
    case Cmd_t::FieldNumber::BACKFLUSH_STOP:
      handle(cmd.get_backflush_stop());
      break;
    case Cmd_t::FieldNumber::RINSE_START:
      handle(cmd.get_rinse_start());
      break;
    case Cmd_t::FieldNumber::RINSE_STOP:
      handle(cmd.get_rinse_stop());
      break;
    case Cmd_t::FieldNumber::BREW_TARGET_SET:
      handle(cmd.get_brew_target_set());
      break;
    case Cmd_t::FieldNumber::SETPOINT_SET:
      handle(cmd.get_setpoint_set());
      break;
    default:
      return "Logic error: Unhandled switch case";
    }

    return std::nullopt;
  };

  std::optional<std::string>
  handle( // NOLINT(readability-convert-member-functions-to-static)
      EmbeddedProto::ReadBufferFixedSize<MsgBufSize> &buf,
      Response<UUIDSize, ERROR_MESSAGE_SIZE> &resp) {
    std::optional<std::string> error = std::nullopt;

    // Deserialize
    auto status = cmd.deserialize(buf);
    buf.clear();

    auto requestID = cmd.get_request_id();

    // Handle deserialization errors
    //
    // NOLINTBEGIN(bugprone-branch-clone)
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
    }
    // NOLINTEND(bugprone-branch-clone)

    // Dispatch to API handler
    if (!error) {
      error = handle(cmd);
    }
    cmd.clear();

    // Set response
    if (error) {
      auto fieldString = resp.mutable_error();
      auto errorChar = error.value().c_str();
      fieldString.set(errorChar, strlen(errorChar) > ERROR_MESSAGE_SIZE
                                     ? ERROR_MESSAGE_SIZE
                                     : strlen(errorChar));
      resp.set_error(fieldString);
    } else {
      resp.set_OK(resp.mutable_OK());
    }
    resp.set_request_id(requestID);

    return error;
  };
};
