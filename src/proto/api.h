// This file is generated. Please do not edit!
#ifndef API_H
#define API_H

#include <Defines.h>
#include <Errors.h>
#include <FieldStringBytes.h>
#include <Fields.h>
#include <MessageInterface.h>
#include <MessageSizeCalculator.h>
#include <ReadBufferSection.h>
#include <RepeatedFieldFixedSize.h>
#include <WireFormatter.h>
#include <cstdint>
#include <limits>

// Include external proto definitions
#include "config.h"

template <uint32_t FloatSensorReading_error_LENGTH>
class FloatSensorReading final : public ::EmbeddedProto::MessageInterface {
public:
  FloatSensorReading() = default;
  FloatSensorReading(const FloatSensorReading &rhs) {
    if (rhs.get_which_value_or_error() != which_value_or_error_) {
      // First delete the old object in the oneof.
      clear_value_or_error();
    }

    switch (rhs.get_which_value_or_error()) {
    case FieldNumber::VALUE:
      set_value(rhs.get_value());
      break;

    case FieldNumber::ERROR:
      set_error(rhs.get_error());
      break;

    default:
      break;
    }
  }

  FloatSensorReading(const FloatSensorReading &&rhs) noexcept {
    if (rhs.get_which_value_or_error() != which_value_or_error_) {
      // First delete the old object in the oneof.
      clear_value_or_error();
    }

    switch (rhs.get_which_value_or_error()) {
    case FieldNumber::VALUE:
      set_value(rhs.get_value());
      break;

    case FieldNumber::ERROR:
      set_error(rhs.get_error());
      break;

    default:
      break;
    }
  }

  ~FloatSensorReading() override = default;

  enum class FieldNumber : uint32_t { NOT_SET = 0, VALUE = 1, ERROR = 2 };

  FloatSensorReading &operator=(const FloatSensorReading &rhs) {
    if (rhs.get_which_value_or_error() != which_value_or_error_) {
      // First delete the old object in the oneof.
      clear_value_or_error();
    }

    switch (rhs.get_which_value_or_error()) {
    case FieldNumber::VALUE:
      set_value(rhs.get_value());
      break;

    case FieldNumber::ERROR:
      set_error(rhs.get_error());
      break;

    default:
      break;
    }

    return *this;
  }

  FloatSensorReading &operator=(const FloatSensorReading &&rhs) noexcept {
    if (rhs.get_which_value_or_error() != which_value_or_error_) {
      // First delete the old object in the oneof.
      clear_value_or_error();
    }

    switch (rhs.get_which_value_or_error()) {
    case FieldNumber::VALUE:
      set_value(rhs.get_value());
      break;

    case FieldNumber::ERROR:
      set_error(rhs.get_error());
      break;

    default:
      break;
    }

    return *this;
  }

  FieldNumber get_which_value_or_error() const { return which_value_or_error_; }

  static constexpr char const *VALUE_NAME = "value";
  inline bool has_value() const {
    return FieldNumber::VALUE == which_value_or_error_;
  }
  inline void clear_value() {
    if (FieldNumber::VALUE == which_value_or_error_) {
      which_value_or_error_ = FieldNumber::NOT_SET;
      ::EmbeddedProto::destroy_at(&value_or_error_.value_);
    }
  }
  inline void set_value(const double &value) {
    if (FieldNumber::VALUE != which_value_or_error_) {
      init_value_or_error(FieldNumber::VALUE);
    }
    value_or_error_.value_ = value;
  }
  inline void set_value(const double &&value) {
    if (FieldNumber::VALUE != which_value_or_error_) {
      init_value_or_error(FieldNumber::VALUE);
    }
    value_or_error_.value_ = value;
  }
  inline const double &get_value() const {
    return value_or_error_.value_.get();
  }
  inline double value() const { return value_or_error_.value_.get(); }

  static constexpr char const *ERROR_NAME = "error";
  inline bool has_error() const {
    return FieldNumber::ERROR == which_value_or_error_;
  }
  inline void clear_error() {
    if (FieldNumber::ERROR == which_value_or_error_) {
      which_value_or_error_ = FieldNumber::NOT_SET;
      value_or_error_.error_.~FieldString();
    }
  }
  inline ::EmbeddedProto::FieldString<FloatSensorReading_error_LENGTH> &
  mutable_error() {
    if (FieldNumber::ERROR != which_value_or_error_) {
      init_value_or_error(FieldNumber::ERROR);
    }
    return value_or_error_.error_;
  }
  inline void
  set_error(const ::EmbeddedProto::FieldString<FloatSensorReading_error_LENGTH>
                &rhs) {
    if (FieldNumber::ERROR != which_value_or_error_) {
      init_value_or_error(FieldNumber::ERROR);
    }
    value_or_error_.error_.set(rhs);
  }
  inline const ::EmbeddedProto::FieldString<FloatSensorReading_error_LENGTH> &
  get_error() const {
    return value_or_error_.error_;
  }
  inline const char *error() const {
    return value_or_error_.error_.get_const();
  }

  ::EmbeddedProto::Error
  serialize(::EmbeddedProto::WriteBufferInterface &buffer) const override {
    ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;

    switch (which_value_or_error_) {
    case FieldNumber::VALUE:
      if (has_value() && (::EmbeddedProto::Error::NO_ERRORS == return_value)) {
        return_value = value_or_error_.value_.serialize_with_id(
            static_cast<uint32_t>(FieldNumber::VALUE), buffer, true);
      }
      break;

    case FieldNumber::ERROR:
      if (::EmbeddedProto::Error::NO_ERRORS == return_value) {
        return_value = value_or_error_.error_.serialize_with_id(
            static_cast<uint32_t>(FieldNumber::ERROR), buffer, true);
      }
      break;

    default:
      break;
    }

    return return_value;
  };

  ::EmbeddedProto::Error
  deserialize(::EmbeddedProto::ReadBufferInterface &buffer) override {
    ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;
    ::EmbeddedProto::WireFormatter::WireType wire_type =
        ::EmbeddedProto::WireFormatter::WireType::VARINT;
    uint32_t id_number = 0;
    FieldNumber id_tag = FieldNumber::NOT_SET;

    ::EmbeddedProto::Error tag_value =
        ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type,
                                                       id_number);
    while ((::EmbeddedProto::Error::NO_ERRORS == return_value) &&
           (::EmbeddedProto::Error::NO_ERRORS == tag_value)) {
      id_tag = static_cast<FieldNumber>(id_number);
      switch (id_tag) {
      case FieldNumber::VALUE:
      case FieldNumber::ERROR:
        return_value = deserialize_value_or_error(id_tag, buffer, wire_type);
        break;

      case FieldNumber::NOT_SET:
        return_value = ::EmbeddedProto::Error::INVALID_FIELD_ID;
        break;

      default:
        return_value = skip_unknown_field(buffer, wire_type);
        break;
      }

      if (::EmbeddedProto::Error::NO_ERRORS == return_value) {
        // Read the next tag.
        tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(
            buffer, wire_type, id_number);
      }
    }

    // When an error was detect while reading the tag but no other errors where
    // found, set it in the return value.
    if ((::EmbeddedProto::Error::NO_ERRORS == return_value) &&
        (::EmbeddedProto::Error::NO_ERRORS != tag_value) &&
        (::EmbeddedProto::Error::END_OF_BUFFER !=
         tag_value)) // The end of the buffer is not an array in this case.
    {
      return_value = tag_value;
    }

    return return_value;
  };

  void clear() override { clear_value_or_error(); }

  static char const *field_number_to_name(const FieldNumber fieldNumber) {
    char const *name = nullptr;
    switch (fieldNumber) {
    case FieldNumber::VALUE:
      name = VALUE_NAME;
      break;
    case FieldNumber::ERROR:
      name = ERROR_NAME;
      break;
    default:
      name = "Invalid FieldNumber";
      break;
    }
    return name;
  }

#ifdef MSG_TO_STRING

  ::EmbeddedProto::string_view
  to_string(::EmbeddedProto::string_view &str) const {
    return this->to_string(str, 0, nullptr, true);
  }

  ::EmbeddedProto::string_view
  to_string(::EmbeddedProto::string_view &str, const uint32_t indent_level,
            char const *name, const bool first_field) const override {
    ::EmbeddedProto::string_view left_chars = str;
    int32_t n_chars_used = 0;

    if (!first_field) {
      // Add a comma behind the previous field.
      n_chars_used = snprintf(left_chars.data, left_chars.size, ",\n");
      if (0 < n_chars_used) {
        // Update the character pointer and characters left in the array.
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }
    }

    if (nullptr != name) {
      if (0 == indent_level) {
        n_chars_used =
            snprintf(left_chars.data, left_chars.size, "\"%s\": {\n", name);
      } else {
        n_chars_used = snprintf(left_chars.data, left_chars.size,
                                "%*s\"%s\": {\n", indent_level, " ", name);
      }
    } else {
      if (0 == indent_level) {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "{\n");
      } else {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s{\n",
                                indent_level, " ");
      }
    }

    if (0 < n_chars_used) {
      left_chars.data += n_chars_used;
      left_chars.size -= n_chars_used;
    }

    left_chars = to_string_value_or_error(left_chars, indent_level + 2, true);

    if (0 == indent_level) {
      n_chars_used = snprintf(left_chars.data, left_chars.size, "\n}");
    } else {
      n_chars_used = snprintf(left_chars.data, left_chars.size, "\n%*s}",
                              indent_level, " ");
    }

    if (0 < n_chars_used) {
      left_chars.data += n_chars_used;
      left_chars.size -= n_chars_used;
    }

    return left_chars;
  }

#endif // End of MSG_TO_STRING

private:
  FieldNumber which_value_or_error_ = FieldNumber::NOT_SET;
  union value_or_error {
    value_or_error() {}
    ~value_or_error() {}
    EmbeddedProto::doublefixed value_;
    ::EmbeddedProto::FieldString<FloatSensorReading_error_LENGTH> error_;
  };
  value_or_error value_or_error_;

  void init_value_or_error(const FieldNumber field_id) {
    if (FieldNumber::NOT_SET != which_value_or_error_) {
      // First delete the old object in the oneof.
      clear_value_or_error();
    }

    // C++11 unions only support nontrivial members when you explicitly call the
    // placement new statement.
    switch (field_id) {
    case FieldNumber::VALUE:
      new (&value_or_error_.value_) EmbeddedProto::doublefixed;
      break;

    case FieldNumber::ERROR:
      new (&value_or_error_.error_)::EmbeddedProto::FieldString<
          FloatSensorReading_error_LENGTH>;
      break;

    default:
      break;
    }

    which_value_or_error_ = field_id;
  }

  void clear_value_or_error() {
    switch (which_value_or_error_) {
    case FieldNumber::VALUE:
      ::EmbeddedProto::destroy_at(&value_or_error_.value_);
      break;
    case FieldNumber::ERROR:
      ::EmbeddedProto::destroy_at(&value_or_error_.error_);
      break;
    default:
      break;
    }
    which_value_or_error_ = FieldNumber::NOT_SET;
  }

  ::EmbeddedProto::Error deserialize_value_or_error(
      const FieldNumber field_id, ::EmbeddedProto::ReadBufferInterface &buffer,
      const ::EmbeddedProto::WireFormatter::WireType wire_type) {
    ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;

    if (field_id != which_value_or_error_) {
      init_value_or_error(field_id);
    }

    switch (which_value_or_error_) {
    case FieldNumber::VALUE:
      return_value =
          value_or_error_.value_.deserialize_check_type(buffer, wire_type);
      break;
    case FieldNumber::ERROR:
      return_value =
          value_or_error_.error_.deserialize_check_type(buffer, wire_type);
      break;
    default:
      break;
    }

    if (::EmbeddedProto::Error::NO_ERRORS != return_value) {
      clear_value_or_error();
    }
    return return_value;
  }

#ifdef MSG_TO_STRING
  ::EmbeddedProto::string_view
  to_string_value_or_error(::EmbeddedProto::string_view &str,
                           const uint32_t indent_level,
                           const bool first_field) const {
    ::EmbeddedProto::string_view left_chars = str;

    switch (which_value_or_error_) {
    case FieldNumber::VALUE:
      left_chars = value_or_error_.value_.to_string(left_chars, indent_level,
                                                    VALUE_NAME, first_field);
      break;
    case FieldNumber::ERROR:
      left_chars = value_or_error_.error_.to_string(left_chars, indent_level,
                                                    ERROR_NAME, first_field);
      break;
    default:
      break;
    }

    return left_chars;
  }

#endif // End of MSG_TO_STRING
};

template <uint32_t LogMessage_msg_LENGTH>
class LogMessage final : public ::EmbeddedProto::MessageInterface {
public:
  LogMessage() = default;
  LogMessage(const LogMessage &rhs) {
    set_logLevel(rhs.get_logLevel());
    set_msg(rhs.get_msg());
  }

  LogMessage(const LogMessage &&rhs) noexcept {
    set_logLevel(rhs.get_logLevel());
    set_msg(rhs.get_msg());
  }

  ~LogMessage() override = default;

  enum class LogLevel : uint32_t { ERROR = 0, INFO = 1, DEBUG = 2 };

  enum class FieldNumber : uint32_t { NOT_SET = 0, LOGLEVEL = 1, MSG = 2 };

  LogMessage &operator=(const LogMessage &rhs) {
    set_logLevel(rhs.get_logLevel());
    set_msg(rhs.get_msg());
    return *this;
  }

  LogMessage &operator=(const LogMessage &&rhs) noexcept {
    set_logLevel(rhs.get_logLevel());
    set_msg(rhs.get_msg());
    return *this;
  }

  static constexpr char const *LOGLEVEL_NAME = "logLevel";
  inline void clear_logLevel() { logLevel_.clear(); }
  inline void set_logLevel(const LogLevel &value) { logLevel_ = value; }
  inline void set_logLevel(const LogLevel &&value) { logLevel_ = value; }
  inline const LogLevel &get_logLevel() const { return logLevel_.get(); }
  inline LogLevel logLevel() const { return logLevel_.get(); }

  static constexpr char const *MSG_NAME = "msg";
  inline void clear_msg() { msg_.clear(); }
  inline ::EmbeddedProto::FieldString<LogMessage_msg_LENGTH> &mutable_msg() {
    return msg_;
  }
  inline void
  set_msg(const ::EmbeddedProto::FieldString<LogMessage_msg_LENGTH> &rhs) {
    msg_.set(rhs);
  }
  inline const ::EmbeddedProto::FieldString<LogMessage_msg_LENGTH> &
  get_msg() const {
    return msg_;
  }
  inline const char *msg() const { return msg_.get_const(); }

  ::EmbeddedProto::Error
  serialize(::EmbeddedProto::WriteBufferInterface &buffer) const override {
    ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;

    if ((static_cast<LogLevel>(0) != logLevel_.get()) &&
        (::EmbeddedProto::Error::NO_ERRORS == return_value)) {
      return_value = logLevel_.serialize_with_id(
          static_cast<uint32_t>(FieldNumber::LOGLEVEL), buffer, false);
    }

    if (::EmbeddedProto::Error::NO_ERRORS == return_value) {
      return_value = msg_.serialize_with_id(
          static_cast<uint32_t>(FieldNumber::MSG), buffer, false);
    }

    return return_value;
  };

  ::EmbeddedProto::Error
  deserialize(::EmbeddedProto::ReadBufferInterface &buffer) override {
    ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;
    ::EmbeddedProto::WireFormatter::WireType wire_type =
        ::EmbeddedProto::WireFormatter::WireType::VARINT;
    uint32_t id_number = 0;
    FieldNumber id_tag = FieldNumber::NOT_SET;

    ::EmbeddedProto::Error tag_value =
        ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type,
                                                       id_number);
    while ((::EmbeddedProto::Error::NO_ERRORS == return_value) &&
           (::EmbeddedProto::Error::NO_ERRORS == tag_value)) {
      id_tag = static_cast<FieldNumber>(id_number);
      switch (id_tag) {
      case FieldNumber::LOGLEVEL:
        return_value = logLevel_.deserialize_check_type(buffer, wire_type);
        break;

      case FieldNumber::MSG:
        return_value = msg_.deserialize_check_type(buffer, wire_type);
        break;

      case FieldNumber::NOT_SET:
        return_value = ::EmbeddedProto::Error::INVALID_FIELD_ID;
        break;

      default:
        return_value = skip_unknown_field(buffer, wire_type);
        break;
      }

      if (::EmbeddedProto::Error::NO_ERRORS == return_value) {
        // Read the next tag.
        tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(
            buffer, wire_type, id_number);
      }
    }

    // When an error was detect while reading the tag but no other errors where
    // found, set it in the return value.
    if ((::EmbeddedProto::Error::NO_ERRORS == return_value) &&
        (::EmbeddedProto::Error::NO_ERRORS != tag_value) &&
        (::EmbeddedProto::Error::END_OF_BUFFER !=
         tag_value)) // The end of the buffer is not an array in this case.
    {
      return_value = tag_value;
    }

    return return_value;
  };

  void clear() override {
    clear_logLevel();
    clear_msg();
  }

  static char const *field_number_to_name(const FieldNumber fieldNumber) {
    char const *name = nullptr;
    switch (fieldNumber) {
    case FieldNumber::LOGLEVEL:
      name = LOGLEVEL_NAME;
      break;
    case FieldNumber::MSG:
      name = MSG_NAME;
      break;
    default:
      name = "Invalid FieldNumber";
      break;
    }
    return name;
  }

#ifdef MSG_TO_STRING

  ::EmbeddedProto::string_view
  to_string(::EmbeddedProto::string_view &str) const {
    return this->to_string(str, 0, nullptr, true);
  }

  ::EmbeddedProto::string_view
  to_string(::EmbeddedProto::string_view &str, const uint32_t indent_level,
            char const *name, const bool first_field) const override {
    ::EmbeddedProto::string_view left_chars = str;
    int32_t n_chars_used = 0;

    if (!first_field) {
      // Add a comma behind the previous field.
      n_chars_used = snprintf(left_chars.data, left_chars.size, ",\n");
      if (0 < n_chars_used) {
        // Update the character pointer and characters left in the array.
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }
    }

    if (nullptr != name) {
      if (0 == indent_level) {
        n_chars_used =
            snprintf(left_chars.data, left_chars.size, "\"%s\": {\n", name);
      } else {
        n_chars_used = snprintf(left_chars.data, left_chars.size,
                                "%*s\"%s\": {\n", indent_level, " ", name);
      }
    } else {
      if (0 == indent_level) {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "{\n");
      } else {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s{\n",
                                indent_level, " ");
      }
    }

    if (0 < n_chars_used) {
      left_chars.data += n_chars_used;
      left_chars.size -= n_chars_used;
    }

    left_chars =
        logLevel_.to_string(left_chars, indent_level + 2, LOGLEVEL_NAME, true);
    left_chars = msg_.to_string(left_chars, indent_level + 2, MSG_NAME, false);

    if (0 == indent_level) {
      n_chars_used = snprintf(left_chars.data, left_chars.size, "\n}");
    } else {
      n_chars_used = snprintf(left_chars.data, left_chars.size, "\n%*s}",
                              indent_level, " ");
    }

    if (0 < n_chars_used) {
      left_chars.data += n_chars_used;
      left_chars.size -= n_chars_used;
    }

    return left_chars;
  }

#endif // End of MSG_TO_STRING

private:
  EmbeddedProto::enumeration<LogLevel> logLevel_ = static_cast<LogLevel>(0);
  ::EmbeddedProto::FieldString<LogMessage_msg_LENGTH> msg_;
};

class PowerOff final : public ::EmbeddedProto::MessageInterface {
public:
  PowerOff() = default;
  PowerOff(const PowerOff &rhs) {}

  PowerOff(const PowerOff &&rhs) noexcept {}

  ~PowerOff() override = default;

  enum class FieldNumber : uint32_t {
    NOT_SET = 0,
  };

  PowerOff &operator=(const PowerOff &rhs) { return *this; }

  PowerOff &operator=(const PowerOff &&rhs) noexcept { return *this; }

  ::EmbeddedProto::Error
  serialize(::EmbeddedProto::WriteBufferInterface &buffer) const override {
    ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;

    return return_value;
  };

  ::EmbeddedProto::Error
  deserialize(::EmbeddedProto::ReadBufferInterface &buffer) override {
    ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;
    ::EmbeddedProto::WireFormatter::WireType wire_type =
        ::EmbeddedProto::WireFormatter::WireType::VARINT;
    uint32_t id_number = 0;
    FieldNumber id_tag = FieldNumber::NOT_SET;

    ::EmbeddedProto::Error tag_value =
        ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type,
                                                       id_number);
    while ((::EmbeddedProto::Error::NO_ERRORS == return_value) &&
           (::EmbeddedProto::Error::NO_ERRORS == tag_value)) {
      id_tag = static_cast<FieldNumber>(id_number);
      switch (id_tag) {
      case FieldNumber::NOT_SET:
        return_value = ::EmbeddedProto::Error::INVALID_FIELD_ID;
        break;

      default:
        return_value = skip_unknown_field(buffer, wire_type);
        break;
      }

      if (::EmbeddedProto::Error::NO_ERRORS == return_value) {
        // Read the next tag.
        tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(
            buffer, wire_type, id_number);
      }
    }

    // When an error was detect while reading the tag but no other errors where
    // found, set it in the return value.
    if ((::EmbeddedProto::Error::NO_ERRORS == return_value) &&
        (::EmbeddedProto::Error::NO_ERRORS != tag_value) &&
        (::EmbeddedProto::Error::END_OF_BUFFER !=
         tag_value)) // The end of the buffer is not an array in this case.
    {
      return_value = tag_value;
    }

    return return_value;
  };

  void clear() override {}

  static char const *field_number_to_name(const FieldNumber fieldNumber) {
    char const *name = nullptr;
    switch (fieldNumber) {
    default:
      name = "Invalid FieldNumber";
      break;
    }
    return name;
  }

#ifdef MSG_TO_STRING

  ::EmbeddedProto::string_view
  to_string(::EmbeddedProto::string_view &str) const {
    return this->to_string(str, 0, nullptr, true);
  }

  ::EmbeddedProto::string_view
  to_string(::EmbeddedProto::string_view &str, const uint32_t indent_level,
            char const *name, const bool first_field) const override {
    ::EmbeddedProto::string_view left_chars = str;
    int32_t n_chars_used = 0;

    if (!first_field) {
      // Add a comma behind the previous field.
      n_chars_used = snprintf(left_chars.data, left_chars.size, ",\n");
      if (0 < n_chars_used) {
        // Update the character pointer and characters left in the array.
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }
    }

    if (nullptr != name) {
      if (0 == indent_level) {
        n_chars_used =
            snprintf(left_chars.data, left_chars.size, "\"%s\": {\n", name);
      } else {
        n_chars_used = snprintf(left_chars.data, left_chars.size,
                                "%*s\"%s\": {\n", indent_level, " ", name);
      }
    } else {
      if (0 == indent_level) {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "{\n");
      } else {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s{\n",
                                indent_level, " ");
      }
    }

    if (0 < n_chars_used) {
      left_chars.data += n_chars_used;
      left_chars.size -= n_chars_used;
    }

    if (0 == indent_level) {
      n_chars_used = snprintf(left_chars.data, left_chars.size, "\n}");
    } else {
      n_chars_used = snprintf(left_chars.data, left_chars.size, "\n%*s}",
                              indent_level, " ");
    }

    if (0 < n_chars_used) {
      left_chars.data += n_chars_used;
      left_chars.size -= n_chars_used;
    }

    return left_chars;
  }

#endif // End of MSG_TO_STRING

private:
};

class PowerOn final : public ::EmbeddedProto::MessageInterface {
public:
  PowerOn() = default;
  PowerOn(const PowerOn &rhs) {}

  PowerOn(const PowerOn &&rhs) noexcept {}

  ~PowerOn() override = default;

  enum class FieldNumber : uint32_t {
    NOT_SET = 0,
  };

  PowerOn &operator=(const PowerOn &rhs) { return *this; }

  PowerOn &operator=(const PowerOn &&rhs) noexcept { return *this; }

  ::EmbeddedProto::Error
  serialize(::EmbeddedProto::WriteBufferInterface &buffer) const override {
    ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;

    return return_value;
  };

  ::EmbeddedProto::Error
  deserialize(::EmbeddedProto::ReadBufferInterface &buffer) override {
    ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;
    ::EmbeddedProto::WireFormatter::WireType wire_type =
        ::EmbeddedProto::WireFormatter::WireType::VARINT;
    uint32_t id_number = 0;
    FieldNumber id_tag = FieldNumber::NOT_SET;

    ::EmbeddedProto::Error tag_value =
        ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type,
                                                       id_number);
    while ((::EmbeddedProto::Error::NO_ERRORS == return_value) &&
           (::EmbeddedProto::Error::NO_ERRORS == tag_value)) {
      id_tag = static_cast<FieldNumber>(id_number);
      switch (id_tag) {
      case FieldNumber::NOT_SET:
        return_value = ::EmbeddedProto::Error::INVALID_FIELD_ID;
        break;

      default:
        return_value = skip_unknown_field(buffer, wire_type);
        break;
      }

      if (::EmbeddedProto::Error::NO_ERRORS == return_value) {
        // Read the next tag.
        tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(
            buffer, wire_type, id_number);
      }
    }

    // When an error was detect while reading the tag but no other errors where
    // found, set it in the return value.
    if ((::EmbeddedProto::Error::NO_ERRORS == return_value) &&
        (::EmbeddedProto::Error::NO_ERRORS != tag_value) &&
        (::EmbeddedProto::Error::END_OF_BUFFER !=
         tag_value)) // The end of the buffer is not an array in this case.
    {
      return_value = tag_value;
    }

    return return_value;
  };

  void clear() override {}

  static char const *field_number_to_name(const FieldNumber fieldNumber) {
    char const *name = nullptr;
    switch (fieldNumber) {
    default:
      name = "Invalid FieldNumber";
      break;
    }
    return name;
  }

#ifdef MSG_TO_STRING

  ::EmbeddedProto::string_view
  to_string(::EmbeddedProto::string_view &str) const {
    return this->to_string(str, 0, nullptr, true);
  }

  ::EmbeddedProto::string_view
  to_string(::EmbeddedProto::string_view &str, const uint32_t indent_level,
            char const *name, const bool first_field) const override {
    ::EmbeddedProto::string_view left_chars = str;
    int32_t n_chars_used = 0;

    if (!first_field) {
      // Add a comma behind the previous field.
      n_chars_used = snprintf(left_chars.data, left_chars.size, ",\n");
      if (0 < n_chars_used) {
        // Update the character pointer and characters left in the array.
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }
    }

    if (nullptr != name) {
      if (0 == indent_level) {
        n_chars_used =
            snprintf(left_chars.data, left_chars.size, "\"%s\": {\n", name);
      } else {
        n_chars_used = snprintf(left_chars.data, left_chars.size,
                                "%*s\"%s\": {\n", indent_level, " ", name);
      }
    } else {
      if (0 == indent_level) {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "{\n");
      } else {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s{\n",
                                indent_level, " ");
      }
    }

    if (0 < n_chars_used) {
      left_chars.data += n_chars_used;
      left_chars.size -= n_chars_used;
    }

    if (0 == indent_level) {
      n_chars_used = snprintf(left_chars.data, left_chars.size, "\n}");
    } else {
      n_chars_used = snprintf(left_chars.data, left_chars.size, "\n%*s}",
                              indent_level, " ");
    }

    if (0 < n_chars_used) {
      left_chars.data += n_chars_used;
      left_chars.size -= n_chars_used;
    }

    return left_chars;
  }

#endif // End of MSG_TO_STRING

private:
};

class StartBrew final : public ::EmbeddedProto::MessageInterface {
public:
  StartBrew() = default;
  StartBrew(const StartBrew &rhs) {}

  StartBrew(const StartBrew &&rhs) noexcept {}

  ~StartBrew() override = default;

  enum class FieldNumber : uint32_t {
    NOT_SET = 0,
  };

  StartBrew &operator=(const StartBrew &rhs) { return *this; }

  StartBrew &operator=(const StartBrew &&rhs) noexcept { return *this; }

  ::EmbeddedProto::Error
  serialize(::EmbeddedProto::WriteBufferInterface &buffer) const override {
    ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;

    return return_value;
  };

  ::EmbeddedProto::Error
  deserialize(::EmbeddedProto::ReadBufferInterface &buffer) override {
    ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;
    ::EmbeddedProto::WireFormatter::WireType wire_type =
        ::EmbeddedProto::WireFormatter::WireType::VARINT;
    uint32_t id_number = 0;
    FieldNumber id_tag = FieldNumber::NOT_SET;

    ::EmbeddedProto::Error tag_value =
        ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type,
                                                       id_number);
    while ((::EmbeddedProto::Error::NO_ERRORS == return_value) &&
           (::EmbeddedProto::Error::NO_ERRORS == tag_value)) {
      id_tag = static_cast<FieldNumber>(id_number);
      switch (id_tag) {
      case FieldNumber::NOT_SET:
        return_value = ::EmbeddedProto::Error::INVALID_FIELD_ID;
        break;

      default:
        return_value = skip_unknown_field(buffer, wire_type);
        break;
      }

      if (::EmbeddedProto::Error::NO_ERRORS == return_value) {
        // Read the next tag.
        tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(
            buffer, wire_type, id_number);
      }
    }

    // When an error was detect while reading the tag but no other errors where
    // found, set it in the return value.
    if ((::EmbeddedProto::Error::NO_ERRORS == return_value) &&
        (::EmbeddedProto::Error::NO_ERRORS != tag_value) &&
        (::EmbeddedProto::Error::END_OF_BUFFER !=
         tag_value)) // The end of the buffer is not an array in this case.
    {
      return_value = tag_value;
    }

    return return_value;
  };

  void clear() override {}

  static char const *field_number_to_name(const FieldNumber fieldNumber) {
    char const *name = nullptr;
    switch (fieldNumber) {
    default:
      name = "Invalid FieldNumber";
      break;
    }
    return name;
  }

#ifdef MSG_TO_STRING

  ::EmbeddedProto::string_view
  to_string(::EmbeddedProto::string_view &str) const {
    return this->to_string(str, 0, nullptr, true);
  }

  ::EmbeddedProto::string_view
  to_string(::EmbeddedProto::string_view &str, const uint32_t indent_level,
            char const *name, const bool first_field) const override {
    ::EmbeddedProto::string_view left_chars = str;
    int32_t n_chars_used = 0;

    if (!first_field) {
      // Add a comma behind the previous field.
      n_chars_used = snprintf(left_chars.data, left_chars.size, ",\n");
      if (0 < n_chars_used) {
        // Update the character pointer and characters left in the array.
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }
    }

    if (nullptr != name) {
      if (0 == indent_level) {
        n_chars_used =
            snprintf(left_chars.data, left_chars.size, "\"%s\": {\n", name);
      } else {
        n_chars_used = snprintf(left_chars.data, left_chars.size,
                                "%*s\"%s\": {\n", indent_level, " ", name);
      }
    } else {
      if (0 == indent_level) {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "{\n");
      } else {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s{\n",
                                indent_level, " ");
      }
    }

    if (0 < n_chars_used) {
      left_chars.data += n_chars_used;
      left_chars.size -= n_chars_used;
    }

    if (0 == indent_level) {
      n_chars_used = snprintf(left_chars.data, left_chars.size, "\n}");
    } else {
      n_chars_used = snprintf(left_chars.data, left_chars.size, "\n%*s}",
                              indent_level, " ");
    }

    if (0 < n_chars_used) {
      left_chars.data += n_chars_used;
      left_chars.size -= n_chars_used;
    }

    return left_chars;
  }

#endif // End of MSG_TO_STRING

private:
};

class StartPump final : public ::EmbeddedProto::MessageInterface {
public:
  StartPump() = default;
  StartPump(const StartPump &rhs) {}

  StartPump(const StartPump &&rhs) noexcept {}

  ~StartPump() override = default;

  enum class FieldNumber : uint32_t {
    NOT_SET = 0,
  };

  StartPump &operator=(const StartPump &rhs) { return *this; }

  StartPump &operator=(const StartPump &&rhs) noexcept { return *this; }

  ::EmbeddedProto::Error
  serialize(::EmbeddedProto::WriteBufferInterface &buffer) const override {
    ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;

    return return_value;
  };

  ::EmbeddedProto::Error
  deserialize(::EmbeddedProto::ReadBufferInterface &buffer) override {
    ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;
    ::EmbeddedProto::WireFormatter::WireType wire_type =
        ::EmbeddedProto::WireFormatter::WireType::VARINT;
    uint32_t id_number = 0;
    FieldNumber id_tag = FieldNumber::NOT_SET;

    ::EmbeddedProto::Error tag_value =
        ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type,
                                                       id_number);
    while ((::EmbeddedProto::Error::NO_ERRORS == return_value) &&
           (::EmbeddedProto::Error::NO_ERRORS == tag_value)) {
      id_tag = static_cast<FieldNumber>(id_number);
      switch (id_tag) {
      case FieldNumber::NOT_SET:
        return_value = ::EmbeddedProto::Error::INVALID_FIELD_ID;
        break;

      default:
        return_value = skip_unknown_field(buffer, wire_type);
        break;
      }

      if (::EmbeddedProto::Error::NO_ERRORS == return_value) {
        // Read the next tag.
        tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(
            buffer, wire_type, id_number);
      }
    }

    // When an error was detect while reading the tag but no other errors where
    // found, set it in the return value.
    if ((::EmbeddedProto::Error::NO_ERRORS == return_value) &&
        (::EmbeddedProto::Error::NO_ERRORS != tag_value) &&
        (::EmbeddedProto::Error::END_OF_BUFFER !=
         tag_value)) // The end of the buffer is not an array in this case.
    {
      return_value = tag_value;
    }

    return return_value;
  };

  void clear() override {}

  static char const *field_number_to_name(const FieldNumber fieldNumber) {
    char const *name = nullptr;
    switch (fieldNumber) {
    default:
      name = "Invalid FieldNumber";
      break;
    }
    return name;
  }

#ifdef MSG_TO_STRING

  ::EmbeddedProto::string_view
  to_string(::EmbeddedProto::string_view &str) const {
    return this->to_string(str, 0, nullptr, true);
  }

  ::EmbeddedProto::string_view
  to_string(::EmbeddedProto::string_view &str, const uint32_t indent_level,
            char const *name, const bool first_field) const override {
    ::EmbeddedProto::string_view left_chars = str;
    int32_t n_chars_used = 0;

    if (!first_field) {
      // Add a comma behind the previous field.
      n_chars_used = snprintf(left_chars.data, left_chars.size, ",\n");
      if (0 < n_chars_used) {
        // Update the character pointer and characters left in the array.
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }
    }

    if (nullptr != name) {
      if (0 == indent_level) {
        n_chars_used =
            snprintf(left_chars.data, left_chars.size, "\"%s\": {\n", name);
      } else {
        n_chars_used = snprintf(left_chars.data, left_chars.size,
                                "%*s\"%s\": {\n", indent_level, " ", name);
      }
    } else {
      if (0 == indent_level) {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "{\n");
      } else {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s{\n",
                                indent_level, " ");
      }
    }

    if (0 < n_chars_used) {
      left_chars.data += n_chars_used;
      left_chars.size -= n_chars_used;
    }

    if (0 == indent_level) {
      n_chars_used = snprintf(left_chars.data, left_chars.size, "\n}");
    } else {
      n_chars_used = snprintf(left_chars.data, left_chars.size, "\n%*s}",
                              indent_level, " ");
    }

    if (0 < n_chars_used) {
      left_chars.data += n_chars_used;
      left_chars.size -= n_chars_used;
    }

    return left_chars;
  }

#endif // End of MSG_TO_STRING

private:
};

class StartSteam final : public ::EmbeddedProto::MessageInterface {
public:
  StartSteam() = default;
  StartSteam(const StartSteam &rhs) {}

  StartSteam(const StartSteam &&rhs) noexcept {}

  ~StartSteam() override = default;

  enum class FieldNumber : uint32_t {
    NOT_SET = 0,
  };

  StartSteam &operator=(const StartSteam &rhs) { return *this; }

  StartSteam &operator=(const StartSteam &&rhs) noexcept { return *this; }

  ::EmbeddedProto::Error
  serialize(::EmbeddedProto::WriteBufferInterface &buffer) const override {
    ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;

    return return_value;
  };

  ::EmbeddedProto::Error
  deserialize(::EmbeddedProto::ReadBufferInterface &buffer) override {
    ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;
    ::EmbeddedProto::WireFormatter::WireType wire_type =
        ::EmbeddedProto::WireFormatter::WireType::VARINT;
    uint32_t id_number = 0;
    FieldNumber id_tag = FieldNumber::NOT_SET;

    ::EmbeddedProto::Error tag_value =
        ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type,
                                                       id_number);
    while ((::EmbeddedProto::Error::NO_ERRORS == return_value) &&
           (::EmbeddedProto::Error::NO_ERRORS == tag_value)) {
      id_tag = static_cast<FieldNumber>(id_number);
      switch (id_tag) {
      case FieldNumber::NOT_SET:
        return_value = ::EmbeddedProto::Error::INVALID_FIELD_ID;
        break;

      default:
        return_value = skip_unknown_field(buffer, wire_type);
        break;
      }

      if (::EmbeddedProto::Error::NO_ERRORS == return_value) {
        // Read the next tag.
        tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(
            buffer, wire_type, id_number);
      }
    }

    // When an error was detect while reading the tag but no other errors where
    // found, set it in the return value.
    if ((::EmbeddedProto::Error::NO_ERRORS == return_value) &&
        (::EmbeddedProto::Error::NO_ERRORS != tag_value) &&
        (::EmbeddedProto::Error::END_OF_BUFFER !=
         tag_value)) // The end of the buffer is not an array in this case.
    {
      return_value = tag_value;
    }

    return return_value;
  };

  void clear() override {}

  static char const *field_number_to_name(const FieldNumber fieldNumber) {
    char const *name = nullptr;
    switch (fieldNumber) {
    default:
      name = "Invalid FieldNumber";
      break;
    }
    return name;
  }

#ifdef MSG_TO_STRING

  ::EmbeddedProto::string_view
  to_string(::EmbeddedProto::string_view &str) const {
    return this->to_string(str, 0, nullptr, true);
  }

  ::EmbeddedProto::string_view
  to_string(::EmbeddedProto::string_view &str, const uint32_t indent_level,
            char const *name, const bool first_field) const override {
    ::EmbeddedProto::string_view left_chars = str;
    int32_t n_chars_used = 0;

    if (!first_field) {
      // Add a comma behind the previous field.
      n_chars_used = snprintf(left_chars.data, left_chars.size, ",\n");
      if (0 < n_chars_used) {
        // Update the character pointer and characters left in the array.
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }
    }

    if (nullptr != name) {
      if (0 == indent_level) {
        n_chars_used =
            snprintf(left_chars.data, left_chars.size, "\"%s\": {\n", name);
      } else {
        n_chars_used = snprintf(left_chars.data, left_chars.size,
                                "%*s\"%s\": {\n", indent_level, " ", name);
      }
    } else {
      if (0 == indent_level) {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "{\n");
      } else {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s{\n",
                                indent_level, " ");
      }
    }

    if (0 < n_chars_used) {
      left_chars.data += n_chars_used;
      left_chars.size -= n_chars_used;
    }

    if (0 == indent_level) {
      n_chars_used = snprintf(left_chars.data, left_chars.size, "\n}");
    } else {
      n_chars_used = snprintf(left_chars.data, left_chars.size, "\n%*s}",
                              indent_level, " ");
    }

    if (0 < n_chars_used) {
      left_chars.data += n_chars_used;
      left_chars.size -= n_chars_used;
    }

    return left_chars;
  }

#endif // End of MSG_TO_STRING

private:
};

class StopBrew final : public ::EmbeddedProto::MessageInterface {
public:
  StopBrew() = default;
  StopBrew(const StopBrew &rhs) {}

  StopBrew(const StopBrew &&rhs) noexcept {}

  ~StopBrew() override = default;

  enum class FieldNumber : uint32_t {
    NOT_SET = 0,
  };

  StopBrew &operator=(const StopBrew &rhs) { return *this; }

  StopBrew &operator=(const StopBrew &&rhs) noexcept { return *this; }

  ::EmbeddedProto::Error
  serialize(::EmbeddedProto::WriteBufferInterface &buffer) const override {
    ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;

    return return_value;
  };

  ::EmbeddedProto::Error
  deserialize(::EmbeddedProto::ReadBufferInterface &buffer) override {
    ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;
    ::EmbeddedProto::WireFormatter::WireType wire_type =
        ::EmbeddedProto::WireFormatter::WireType::VARINT;
    uint32_t id_number = 0;
    FieldNumber id_tag = FieldNumber::NOT_SET;

    ::EmbeddedProto::Error tag_value =
        ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type,
                                                       id_number);
    while ((::EmbeddedProto::Error::NO_ERRORS == return_value) &&
           (::EmbeddedProto::Error::NO_ERRORS == tag_value)) {
      id_tag = static_cast<FieldNumber>(id_number);
      switch (id_tag) {
      case FieldNumber::NOT_SET:
        return_value = ::EmbeddedProto::Error::INVALID_FIELD_ID;
        break;

      default:
        return_value = skip_unknown_field(buffer, wire_type);
        break;
      }

      if (::EmbeddedProto::Error::NO_ERRORS == return_value) {
        // Read the next tag.
        tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(
            buffer, wire_type, id_number);
      }
    }

    // When an error was detect while reading the tag but no other errors where
    // found, set it in the return value.
    if ((::EmbeddedProto::Error::NO_ERRORS == return_value) &&
        (::EmbeddedProto::Error::NO_ERRORS != tag_value) &&
        (::EmbeddedProto::Error::END_OF_BUFFER !=
         tag_value)) // The end of the buffer is not an array in this case.
    {
      return_value = tag_value;
    }

    return return_value;
  };

  void clear() override {}

  static char const *field_number_to_name(const FieldNumber fieldNumber) {
    char const *name = nullptr;
    switch (fieldNumber) {
    default:
      name = "Invalid FieldNumber";
      break;
    }
    return name;
  }

#ifdef MSG_TO_STRING

  ::EmbeddedProto::string_view
  to_string(::EmbeddedProto::string_view &str) const {
    return this->to_string(str, 0, nullptr, true);
  }

  ::EmbeddedProto::string_view
  to_string(::EmbeddedProto::string_view &str, const uint32_t indent_level,
            char const *name, const bool first_field) const override {
    ::EmbeddedProto::string_view left_chars = str;
    int32_t n_chars_used = 0;

    if (!first_field) {
      // Add a comma behind the previous field.
      n_chars_used = snprintf(left_chars.data, left_chars.size, ",\n");
      if (0 < n_chars_used) {
        // Update the character pointer and characters left in the array.
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }
    }

    if (nullptr != name) {
      if (0 == indent_level) {
        n_chars_used =
            snprintf(left_chars.data, left_chars.size, "\"%s\": {\n", name);
      } else {
        n_chars_used = snprintf(left_chars.data, left_chars.size,
                                "%*s\"%s\": {\n", indent_level, " ", name);
      }
    } else {
      if (0 == indent_level) {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "{\n");
      } else {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s{\n",
                                indent_level, " ");
      }
    }

    if (0 < n_chars_used) {
      left_chars.data += n_chars_used;
      left_chars.size -= n_chars_used;
    }

    if (0 == indent_level) {
      n_chars_used = snprintf(left_chars.data, left_chars.size, "\n}");
    } else {
      n_chars_used = snprintf(left_chars.data, left_chars.size, "\n%*s}",
                              indent_level, " ");
    }

    if (0 < n_chars_used) {
      left_chars.data += n_chars_used;
      left_chars.size -= n_chars_used;
    }

    return left_chars;
  }

#endif // End of MSG_TO_STRING

private:
};

class StopPump final : public ::EmbeddedProto::MessageInterface {
public:
  StopPump() = default;
  StopPump(const StopPump &rhs) {}

  StopPump(const StopPump &&rhs) noexcept {}

  ~StopPump() override = default;

  enum class FieldNumber : uint32_t {
    NOT_SET = 0,
  };

  StopPump &operator=(const StopPump &rhs) { return *this; }

  StopPump &operator=(const StopPump &&rhs) noexcept { return *this; }

  ::EmbeddedProto::Error
  serialize(::EmbeddedProto::WriteBufferInterface &buffer) const override {
    ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;

    return return_value;
  };

  ::EmbeddedProto::Error
  deserialize(::EmbeddedProto::ReadBufferInterface &buffer) override {
    ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;
    ::EmbeddedProto::WireFormatter::WireType wire_type =
        ::EmbeddedProto::WireFormatter::WireType::VARINT;
    uint32_t id_number = 0;
    FieldNumber id_tag = FieldNumber::NOT_SET;

    ::EmbeddedProto::Error tag_value =
        ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type,
                                                       id_number);
    while ((::EmbeddedProto::Error::NO_ERRORS == return_value) &&
           (::EmbeddedProto::Error::NO_ERRORS == tag_value)) {
      id_tag = static_cast<FieldNumber>(id_number);
      switch (id_tag) {
      case FieldNumber::NOT_SET:
        return_value = ::EmbeddedProto::Error::INVALID_FIELD_ID;
        break;

      default:
        return_value = skip_unknown_field(buffer, wire_type);
        break;
      }

      if (::EmbeddedProto::Error::NO_ERRORS == return_value) {
        // Read the next tag.
        tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(
            buffer, wire_type, id_number);
      }
    }

    // When an error was detect while reading the tag but no other errors where
    // found, set it in the return value.
    if ((::EmbeddedProto::Error::NO_ERRORS == return_value) &&
        (::EmbeddedProto::Error::NO_ERRORS != tag_value) &&
        (::EmbeddedProto::Error::END_OF_BUFFER !=
         tag_value)) // The end of the buffer is not an array in this case.
    {
      return_value = tag_value;
    }

    return return_value;
  };

  void clear() override {}

  static char const *field_number_to_name(const FieldNumber fieldNumber) {
    char const *name = nullptr;
    switch (fieldNumber) {
    default:
      name = "Invalid FieldNumber";
      break;
    }
    return name;
  }

#ifdef MSG_TO_STRING

  ::EmbeddedProto::string_view
  to_string(::EmbeddedProto::string_view &str) const {
    return this->to_string(str, 0, nullptr, true);
  }

  ::EmbeddedProto::string_view
  to_string(::EmbeddedProto::string_view &str, const uint32_t indent_level,
            char const *name, const bool first_field) const override {
    ::EmbeddedProto::string_view left_chars = str;
    int32_t n_chars_used = 0;

    if (!first_field) {
      // Add a comma behind the previous field.
      n_chars_used = snprintf(left_chars.data, left_chars.size, ",\n");
      if (0 < n_chars_used) {
        // Update the character pointer and characters left in the array.
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }
    }

    if (nullptr != name) {
      if (0 == indent_level) {
        n_chars_used =
            snprintf(left_chars.data, left_chars.size, "\"%s\": {\n", name);
      } else {
        n_chars_used = snprintf(left_chars.data, left_chars.size,
                                "%*s\"%s\": {\n", indent_level, " ", name);
      }
    } else {
      if (0 == indent_level) {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "{\n");
      } else {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s{\n",
                                indent_level, " ");
      }
    }

    if (0 < n_chars_used) {
      left_chars.data += n_chars_used;
      left_chars.size -= n_chars_used;
    }

    if (0 == indent_level) {
      n_chars_used = snprintf(left_chars.data, left_chars.size, "\n}");
    } else {
      n_chars_used = snprintf(left_chars.data, left_chars.size, "\n%*s}",
                              indent_level, " ");
    }

    if (0 < n_chars_used) {
      left_chars.data += n_chars_used;
      left_chars.size -= n_chars_used;
    }

    return left_chars;
  }

#endif // End of MSG_TO_STRING

private:
};

class StopSteam final : public ::EmbeddedProto::MessageInterface {
public:
  StopSteam() = default;
  StopSteam(const StopSteam &rhs) {}

  StopSteam(const StopSteam &&rhs) noexcept {}

  ~StopSteam() override = default;

  enum class FieldNumber : uint32_t {
    NOT_SET = 0,
  };

  StopSteam &operator=(const StopSteam &rhs) { return *this; }

  StopSteam &operator=(const StopSteam &&rhs) noexcept { return *this; }

  ::EmbeddedProto::Error
  serialize(::EmbeddedProto::WriteBufferInterface &buffer) const override {
    ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;

    return return_value;
  };

  ::EmbeddedProto::Error
  deserialize(::EmbeddedProto::ReadBufferInterface &buffer) override {
    ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;
    ::EmbeddedProto::WireFormatter::WireType wire_type =
        ::EmbeddedProto::WireFormatter::WireType::VARINT;
    uint32_t id_number = 0;
    FieldNumber id_tag = FieldNumber::NOT_SET;

    ::EmbeddedProto::Error tag_value =
        ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type,
                                                       id_number);
    while ((::EmbeddedProto::Error::NO_ERRORS == return_value) &&
           (::EmbeddedProto::Error::NO_ERRORS == tag_value)) {
      id_tag = static_cast<FieldNumber>(id_number);
      switch (id_tag) {
      case FieldNumber::NOT_SET:
        return_value = ::EmbeddedProto::Error::INVALID_FIELD_ID;
        break;

      default:
        return_value = skip_unknown_field(buffer, wire_type);
        break;
      }

      if (::EmbeddedProto::Error::NO_ERRORS == return_value) {
        // Read the next tag.
        tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(
            buffer, wire_type, id_number);
      }
    }

    // When an error was detect while reading the tag but no other errors where
    // found, set it in the return value.
    if ((::EmbeddedProto::Error::NO_ERRORS == return_value) &&
        (::EmbeddedProto::Error::NO_ERRORS != tag_value) &&
        (::EmbeddedProto::Error::END_OF_BUFFER !=
         tag_value)) // The end of the buffer is not an array in this case.
    {
      return_value = tag_value;
    }

    return return_value;
  };

  void clear() override {}

  static char const *field_number_to_name(const FieldNumber fieldNumber) {
    char const *name = nullptr;
    switch (fieldNumber) {
    default:
      name = "Invalid FieldNumber";
      break;
    }
    return name;
  }

#ifdef MSG_TO_STRING

  ::EmbeddedProto::string_view
  to_string(::EmbeddedProto::string_view &str) const {
    return this->to_string(str, 0, nullptr, true);
  }

  ::EmbeddedProto::string_view
  to_string(::EmbeddedProto::string_view &str, const uint32_t indent_level,
            char const *name, const bool first_field) const override {
    ::EmbeddedProto::string_view left_chars = str;
    int32_t n_chars_used = 0;

    if (!first_field) {
      // Add a comma behind the previous field.
      n_chars_used = snprintf(left_chars.data, left_chars.size, ",\n");
      if (0 < n_chars_used) {
        // Update the character pointer and characters left in the array.
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }
    }

    if (nullptr != name) {
      if (0 == indent_level) {
        n_chars_used =
            snprintf(left_chars.data, left_chars.size, "\"%s\": {\n", name);
      } else {
        n_chars_used = snprintf(left_chars.data, left_chars.size,
                                "%*s\"%s\": {\n", indent_level, " ", name);
      }
    } else {
      if (0 == indent_level) {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "{\n");
      } else {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s{\n",
                                indent_level, " ");
      }
    }

    if (0 < n_chars_used) {
      left_chars.data += n_chars_used;
      left_chars.size -= n_chars_used;
    }

    if (0 == indent_level) {
      n_chars_used = snprintf(left_chars.data, left_chars.size, "\n}");
    } else {
      n_chars_used = snprintf(left_chars.data, left_chars.size, "\n%*s}",
                              indent_level, " ");
    }

    if (0 < n_chars_used) {
      left_chars.data += n_chars_used;
      left_chars.size -= n_chars_used;
    }

    return left_chars;
  }

#endif // End of MSG_TO_STRING

private:
};

template <uint32_t Command_request_id_LENGTH>
class Command final : public ::EmbeddedProto::MessageInterface {
public:
  Command() = default;
  Command(const Command &rhs) {
    set_request_id(rhs.get_request_id());
    if (rhs.get_which_command_oneof() != which_command_oneof_) {
      // First delete the old object in the oneof.
      clear_command_oneof();
    }

    switch (rhs.get_which_command_oneof()) {
    case FieldNumber::POWER_ON:
      set_power_on(rhs.get_power_on());
      break;

    case FieldNumber::POWER_OFF:
      set_power_off(rhs.get_power_off());
      break;

    case FieldNumber::START_BREW:
      set_start_brew(rhs.get_start_brew());
      break;

    case FieldNumber::STOP_BREW:
      set_stop_brew(rhs.get_stop_brew());
      break;

    case FieldNumber::START_PUMP:
      set_start_pump(rhs.get_start_pump());
      break;

    case FieldNumber::STOP_PUMP:
      set_stop_pump(rhs.get_stop_pump());
      break;

    case FieldNumber::START_STEAM:
      set_start_steam(rhs.get_start_steam());
      break;

    case FieldNumber::STOP_STEAM:
      set_stop_steam(rhs.get_stop_steam());
      break;

    case FieldNumber::CONFIG:
      set_config(rhs.get_config());
      break;

    default:
      break;
    }
  }

  Command(const Command &&rhs) noexcept {
    set_request_id(rhs.get_request_id());
    if (rhs.get_which_command_oneof() != which_command_oneof_) {
      // First delete the old object in the oneof.
      clear_command_oneof();
    }

    switch (rhs.get_which_command_oneof()) {
    case FieldNumber::POWER_ON:
      set_power_on(rhs.get_power_on());
      break;

    case FieldNumber::POWER_OFF:
      set_power_off(rhs.get_power_off());
      break;

    case FieldNumber::START_BREW:
      set_start_brew(rhs.get_start_brew());
      break;

    case FieldNumber::STOP_BREW:
      set_stop_brew(rhs.get_stop_brew());
      break;

    case FieldNumber::START_PUMP:
      set_start_pump(rhs.get_start_pump());
      break;

    case FieldNumber::STOP_PUMP:
      set_stop_pump(rhs.get_stop_pump());
      break;

    case FieldNumber::START_STEAM:
      set_start_steam(rhs.get_start_steam());
      break;

    case FieldNumber::STOP_STEAM:
      set_stop_steam(rhs.get_stop_steam());
      break;

    case FieldNumber::CONFIG:
      set_config(rhs.get_config());
      break;

    default:
      break;
    }
  }

  ~Command() override = default;

  enum class FieldNumber : uint32_t {
    NOT_SET = 0,
    REQUEST_ID = 1,
    POWER_ON = 2,
    POWER_OFF = 3,
    START_BREW = 4,
    STOP_BREW = 5,
    START_PUMP = 6,
    STOP_PUMP = 7,
    START_STEAM = 8,
    STOP_STEAM = 9,
    CONFIG = 10
  };

  Command &operator=(const Command &rhs) {
    set_request_id(rhs.get_request_id());
    if (rhs.get_which_command_oneof() != which_command_oneof_) {
      // First delete the old object in the oneof.
      clear_command_oneof();
    }

    switch (rhs.get_which_command_oneof()) {
    case FieldNumber::POWER_ON:
      set_power_on(rhs.get_power_on());
      break;

    case FieldNumber::POWER_OFF:
      set_power_off(rhs.get_power_off());
      break;

    case FieldNumber::START_BREW:
      set_start_brew(rhs.get_start_brew());
      break;

    case FieldNumber::STOP_BREW:
      set_stop_brew(rhs.get_stop_brew());
      break;

    case FieldNumber::START_PUMP:
      set_start_pump(rhs.get_start_pump());
      break;

    case FieldNumber::STOP_PUMP:
      set_stop_pump(rhs.get_stop_pump());
      break;

    case FieldNumber::START_STEAM:
      set_start_steam(rhs.get_start_steam());
      break;

    case FieldNumber::STOP_STEAM:
      set_stop_steam(rhs.get_stop_steam());
      break;

    case FieldNumber::CONFIG:
      set_config(rhs.get_config());
      break;

    default:
      break;
    }

    return *this;
  }

  Command &operator=(const Command &&rhs) noexcept {
    set_request_id(rhs.get_request_id());
    if (rhs.get_which_command_oneof() != which_command_oneof_) {
      // First delete the old object in the oneof.
      clear_command_oneof();
    }

    switch (rhs.get_which_command_oneof()) {
    case FieldNumber::POWER_ON:
      set_power_on(rhs.get_power_on());
      break;

    case FieldNumber::POWER_OFF:
      set_power_off(rhs.get_power_off());
      break;

    case FieldNumber::START_BREW:
      set_start_brew(rhs.get_start_brew());
      break;

    case FieldNumber::STOP_BREW:
      set_stop_brew(rhs.get_stop_brew());
      break;

    case FieldNumber::START_PUMP:
      set_start_pump(rhs.get_start_pump());
      break;

    case FieldNumber::STOP_PUMP:
      set_stop_pump(rhs.get_stop_pump());
      break;

    case FieldNumber::START_STEAM:
      set_start_steam(rhs.get_start_steam());
      break;

    case FieldNumber::STOP_STEAM:
      set_stop_steam(rhs.get_stop_steam());
      break;

    case FieldNumber::CONFIG:
      set_config(rhs.get_config());
      break;

    default:
      break;
    }

    return *this;
  }

  static constexpr char const *REQUEST_ID_NAME = "request_id";
  inline void clear_request_id() { request_id_.clear(); }
  inline ::EmbeddedProto::FieldBytes<Command_request_id_LENGTH> &
  mutable_request_id() {
    return request_id_;
  }
  inline void set_request_id(
      const ::EmbeddedProto::FieldBytes<Command_request_id_LENGTH> &rhs) {
    request_id_.set(rhs);
  }
  inline const ::EmbeddedProto::FieldBytes<Command_request_id_LENGTH> &
  get_request_id() const {
    return request_id_;
  }
  inline const uint8_t *request_id() const { return request_id_.get_const(); }

  FieldNumber get_which_command_oneof() const { return which_command_oneof_; }

  static constexpr char const *POWER_ON_NAME = "power_on";
  inline bool has_power_on() const {
    return FieldNumber::POWER_ON == which_command_oneof_;
  }
  inline void clear_power_on() {
    if (FieldNumber::POWER_ON == which_command_oneof_) {
      which_command_oneof_ = FieldNumber::NOT_SET;
      command_oneof_.power_on_.~PowerOn();
    }
  }
  inline void set_power_on(const PowerOn &value) {
    if (FieldNumber::POWER_ON != which_command_oneof_) {
      init_command_oneof(FieldNumber::POWER_ON);
    }
    command_oneof_.power_on_ = value;
  }
  inline void set_power_on(const PowerOn &&value) {
    if (FieldNumber::POWER_ON != which_command_oneof_) {
      init_command_oneof(FieldNumber::POWER_ON);
    }
    command_oneof_.power_on_ = value;
  }
  inline PowerOn &mutable_power_on() {
    if (FieldNumber::POWER_ON != which_command_oneof_) {
      init_command_oneof(FieldNumber::POWER_ON);
    }
    return command_oneof_.power_on_;
  }
  inline const PowerOn &get_power_on() const {
    return command_oneof_.power_on_;
  }
  inline const PowerOn &power_on() const { return command_oneof_.power_on_; }

  static constexpr char const *POWER_OFF_NAME = "power_off";
  inline bool has_power_off() const {
    return FieldNumber::POWER_OFF == which_command_oneof_;
  }
  inline void clear_power_off() {
    if (FieldNumber::POWER_OFF == which_command_oneof_) {
      which_command_oneof_ = FieldNumber::NOT_SET;
      command_oneof_.power_off_.~PowerOff();
    }
  }
  inline void set_power_off(const PowerOff &value) {
    if (FieldNumber::POWER_OFF != which_command_oneof_) {
      init_command_oneof(FieldNumber::POWER_OFF);
    }
    command_oneof_.power_off_ = value;
  }
  inline void set_power_off(const PowerOff &&value) {
    if (FieldNumber::POWER_OFF != which_command_oneof_) {
      init_command_oneof(FieldNumber::POWER_OFF);
    }
    command_oneof_.power_off_ = value;
  }
  inline PowerOff &mutable_power_off() {
    if (FieldNumber::POWER_OFF != which_command_oneof_) {
      init_command_oneof(FieldNumber::POWER_OFF);
    }
    return command_oneof_.power_off_;
  }
  inline const PowerOff &get_power_off() const {
    return command_oneof_.power_off_;
  }
  inline const PowerOff &power_off() const { return command_oneof_.power_off_; }

  static constexpr char const *START_BREW_NAME = "start_brew";
  inline bool has_start_brew() const {
    return FieldNumber::START_BREW == which_command_oneof_;
  }
  inline void clear_start_brew() {
    if (FieldNumber::START_BREW == which_command_oneof_) {
      which_command_oneof_ = FieldNumber::NOT_SET;
      command_oneof_.start_brew_.~StartBrew();
    }
  }
  inline void set_start_brew(const StartBrew &value) {
    if (FieldNumber::START_BREW != which_command_oneof_) {
      init_command_oneof(FieldNumber::START_BREW);
    }
    command_oneof_.start_brew_ = value;
  }
  inline void set_start_brew(const StartBrew &&value) {
    if (FieldNumber::START_BREW != which_command_oneof_) {
      init_command_oneof(FieldNumber::START_BREW);
    }
    command_oneof_.start_brew_ = value;
  }
  inline StartBrew &mutable_start_brew() {
    if (FieldNumber::START_BREW != which_command_oneof_) {
      init_command_oneof(FieldNumber::START_BREW);
    }
    return command_oneof_.start_brew_;
  }
  inline const StartBrew &get_start_brew() const {
    return command_oneof_.start_brew_;
  }
  inline const StartBrew &start_brew() const {
    return command_oneof_.start_brew_;
  }

  static constexpr char const *STOP_BREW_NAME = "stop_brew";
  inline bool has_stop_brew() const {
    return FieldNumber::STOP_BREW == which_command_oneof_;
  }
  inline void clear_stop_brew() {
    if (FieldNumber::STOP_BREW == which_command_oneof_) {
      which_command_oneof_ = FieldNumber::NOT_SET;
      command_oneof_.stop_brew_.~StopBrew();
    }
  }
  inline void set_stop_brew(const StopBrew &value) {
    if (FieldNumber::STOP_BREW != which_command_oneof_) {
      init_command_oneof(FieldNumber::STOP_BREW);
    }
    command_oneof_.stop_brew_ = value;
  }
  inline void set_stop_brew(const StopBrew &&value) {
    if (FieldNumber::STOP_BREW != which_command_oneof_) {
      init_command_oneof(FieldNumber::STOP_BREW);
    }
    command_oneof_.stop_brew_ = value;
  }
  inline StopBrew &mutable_stop_brew() {
    if (FieldNumber::STOP_BREW != which_command_oneof_) {
      init_command_oneof(FieldNumber::STOP_BREW);
    }
    return command_oneof_.stop_brew_;
  }
  inline const StopBrew &get_stop_brew() const {
    return command_oneof_.stop_brew_;
  }
  inline const StopBrew &stop_brew() const { return command_oneof_.stop_brew_; }

  static constexpr char const *START_PUMP_NAME = "start_pump";
  inline bool has_start_pump() const {
    return FieldNumber::START_PUMP == which_command_oneof_;
  }
  inline void clear_start_pump() {
    if (FieldNumber::START_PUMP == which_command_oneof_) {
      which_command_oneof_ = FieldNumber::NOT_SET;
      command_oneof_.start_pump_.~StartPump();
    }
  }
  inline void set_start_pump(const StartPump &value) {
    if (FieldNumber::START_PUMP != which_command_oneof_) {
      init_command_oneof(FieldNumber::START_PUMP);
    }
    command_oneof_.start_pump_ = value;
  }
  inline void set_start_pump(const StartPump &&value) {
    if (FieldNumber::START_PUMP != which_command_oneof_) {
      init_command_oneof(FieldNumber::START_PUMP);
    }
    command_oneof_.start_pump_ = value;
  }
  inline StartPump &mutable_start_pump() {
    if (FieldNumber::START_PUMP != which_command_oneof_) {
      init_command_oneof(FieldNumber::START_PUMP);
    }
    return command_oneof_.start_pump_;
  }
  inline const StartPump &get_start_pump() const {
    return command_oneof_.start_pump_;
  }
  inline const StartPump &start_pump() const {
    return command_oneof_.start_pump_;
  }

  static constexpr char const *STOP_PUMP_NAME = "stop_pump";
  inline bool has_stop_pump() const {
    return FieldNumber::STOP_PUMP == which_command_oneof_;
  }
  inline void clear_stop_pump() {
    if (FieldNumber::STOP_PUMP == which_command_oneof_) {
      which_command_oneof_ = FieldNumber::NOT_SET;
      command_oneof_.stop_pump_.~StopPump();
    }
  }
  inline void set_stop_pump(const StopPump &value) {
    if (FieldNumber::STOP_PUMP != which_command_oneof_) {
      init_command_oneof(FieldNumber::STOP_PUMP);
    }
    command_oneof_.stop_pump_ = value;
  }
  inline void set_stop_pump(const StopPump &&value) {
    if (FieldNumber::STOP_PUMP != which_command_oneof_) {
      init_command_oneof(FieldNumber::STOP_PUMP);
    }
    command_oneof_.stop_pump_ = value;
  }
  inline StopPump &mutable_stop_pump() {
    if (FieldNumber::STOP_PUMP != which_command_oneof_) {
      init_command_oneof(FieldNumber::STOP_PUMP);
    }
    return command_oneof_.stop_pump_;
  }
  inline const StopPump &get_stop_pump() const {
    return command_oneof_.stop_pump_;
  }
  inline const StopPump &stop_pump() const { return command_oneof_.stop_pump_; }

  static constexpr char const *START_STEAM_NAME = "start_steam";
  inline bool has_start_steam() const {
    return FieldNumber::START_STEAM == which_command_oneof_;
  }
  inline void clear_start_steam() {
    if (FieldNumber::START_STEAM == which_command_oneof_) {
      which_command_oneof_ = FieldNumber::NOT_SET;
      command_oneof_.start_steam_.~StartSteam();
    }
  }
  inline void set_start_steam(const StartSteam &value) {
    if (FieldNumber::START_STEAM != which_command_oneof_) {
      init_command_oneof(FieldNumber::START_STEAM);
    }
    command_oneof_.start_steam_ = value;
  }
  inline void set_start_steam(const StartSteam &&value) {
    if (FieldNumber::START_STEAM != which_command_oneof_) {
      init_command_oneof(FieldNumber::START_STEAM);
    }
    command_oneof_.start_steam_ = value;
  }
  inline StartSteam &mutable_start_steam() {
    if (FieldNumber::START_STEAM != which_command_oneof_) {
      init_command_oneof(FieldNumber::START_STEAM);
    }
    return command_oneof_.start_steam_;
  }
  inline const StartSteam &get_start_steam() const {
    return command_oneof_.start_steam_;
  }
  inline const StartSteam &start_steam() const {
    return command_oneof_.start_steam_;
  }

  static constexpr char const *STOP_STEAM_NAME = "stop_steam";
  inline bool has_stop_steam() const {
    return FieldNumber::STOP_STEAM == which_command_oneof_;
  }
  inline void clear_stop_steam() {
    if (FieldNumber::STOP_STEAM == which_command_oneof_) {
      which_command_oneof_ = FieldNumber::NOT_SET;
      command_oneof_.stop_steam_.~StopSteam();
    }
  }
  inline void set_stop_steam(const StopSteam &value) {
    if (FieldNumber::STOP_STEAM != which_command_oneof_) {
      init_command_oneof(FieldNumber::STOP_STEAM);
    }
    command_oneof_.stop_steam_ = value;
  }
  inline void set_stop_steam(const StopSteam &&value) {
    if (FieldNumber::STOP_STEAM != which_command_oneof_) {
      init_command_oneof(FieldNumber::STOP_STEAM);
    }
    command_oneof_.stop_steam_ = value;
  }
  inline StopSteam &mutable_stop_steam() {
    if (FieldNumber::STOP_STEAM != which_command_oneof_) {
      init_command_oneof(FieldNumber::STOP_STEAM);
    }
    return command_oneof_.stop_steam_;
  }
  inline const StopSteam &get_stop_steam() const {
    return command_oneof_.stop_steam_;
  }
  inline const StopSteam &stop_steam() const {
    return command_oneof_.stop_steam_;
  }

  static constexpr char const *CONFIG_NAME = "config";
  inline bool has_config() const {
    return FieldNumber::CONFIG == which_command_oneof_;
  }
  inline void clear_config() {
    if (FieldNumber::CONFIG == which_command_oneof_) {
      which_command_oneof_ = FieldNumber::NOT_SET;
      command_oneof_.config_.~Config();
    }
  }
  inline void set_config(const Config &value) {
    if (FieldNumber::CONFIG != which_command_oneof_) {
      init_command_oneof(FieldNumber::CONFIG);
    }
    command_oneof_.config_ = value;
  }
  inline void set_config(const Config &&value) {
    if (FieldNumber::CONFIG != which_command_oneof_) {
      init_command_oneof(FieldNumber::CONFIG);
    }
    command_oneof_.config_ = value;
  }
  inline Config &mutable_config() {
    if (FieldNumber::CONFIG != which_command_oneof_) {
      init_command_oneof(FieldNumber::CONFIG);
    }
    return command_oneof_.config_;
  }
  inline const Config &get_config() const { return command_oneof_.config_; }
  inline const Config &config() const { return command_oneof_.config_; }

  ::EmbeddedProto::Error
  serialize(::EmbeddedProto::WriteBufferInterface &buffer) const override {
    ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;

    if (::EmbeddedProto::Error::NO_ERRORS == return_value) {
      return_value = request_id_.serialize_with_id(
          static_cast<uint32_t>(FieldNumber::REQUEST_ID), buffer, false);
    }

    switch (which_command_oneof_) {
    case FieldNumber::POWER_ON:
      if (has_power_on() &&
          (::EmbeddedProto::Error::NO_ERRORS == return_value)) {
        return_value = command_oneof_.power_on_.serialize_with_id(
            static_cast<uint32_t>(FieldNumber::POWER_ON), buffer, true);
      }
      break;

    case FieldNumber::POWER_OFF:
      if (has_power_off() &&
          (::EmbeddedProto::Error::NO_ERRORS == return_value)) {
        return_value = command_oneof_.power_off_.serialize_with_id(
            static_cast<uint32_t>(FieldNumber::POWER_OFF), buffer, true);
      }
      break;

    case FieldNumber::START_BREW:
      if (has_start_brew() &&
          (::EmbeddedProto::Error::NO_ERRORS == return_value)) {
        return_value = command_oneof_.start_brew_.serialize_with_id(
            static_cast<uint32_t>(FieldNumber::START_BREW), buffer, true);
      }
      break;

    case FieldNumber::STOP_BREW:
      if (has_stop_brew() &&
          (::EmbeddedProto::Error::NO_ERRORS == return_value)) {
        return_value = command_oneof_.stop_brew_.serialize_with_id(
            static_cast<uint32_t>(FieldNumber::STOP_BREW), buffer, true);
      }
      break;

    case FieldNumber::START_PUMP:
      if (has_start_pump() &&
          (::EmbeddedProto::Error::NO_ERRORS == return_value)) {
        return_value = command_oneof_.start_pump_.serialize_with_id(
            static_cast<uint32_t>(FieldNumber::START_PUMP), buffer, true);
      }
      break;

    case FieldNumber::STOP_PUMP:
      if (has_stop_pump() &&
          (::EmbeddedProto::Error::NO_ERRORS == return_value)) {
        return_value = command_oneof_.stop_pump_.serialize_with_id(
            static_cast<uint32_t>(FieldNumber::STOP_PUMP), buffer, true);
      }
      break;

    case FieldNumber::START_STEAM:
      if (has_start_steam() &&
          (::EmbeddedProto::Error::NO_ERRORS == return_value)) {
        return_value = command_oneof_.start_steam_.serialize_with_id(
            static_cast<uint32_t>(FieldNumber::START_STEAM), buffer, true);
      }
      break;

    case FieldNumber::STOP_STEAM:
      if (has_stop_steam() &&
          (::EmbeddedProto::Error::NO_ERRORS == return_value)) {
        return_value = command_oneof_.stop_steam_.serialize_with_id(
            static_cast<uint32_t>(FieldNumber::STOP_STEAM), buffer, true);
      }
      break;

    case FieldNumber::CONFIG:
      if (has_config() && (::EmbeddedProto::Error::NO_ERRORS == return_value)) {
        return_value = command_oneof_.config_.serialize_with_id(
            static_cast<uint32_t>(FieldNumber::CONFIG), buffer, true);
      }
      break;

    default:
      break;
    }

    return return_value;
  };

  ::EmbeddedProto::Error
  deserialize(::EmbeddedProto::ReadBufferInterface &buffer) override {
    ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;
    ::EmbeddedProto::WireFormatter::WireType wire_type =
        ::EmbeddedProto::WireFormatter::WireType::VARINT;
    uint32_t id_number = 0;
    FieldNumber id_tag = FieldNumber::NOT_SET;

    ::EmbeddedProto::Error tag_value =
        ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type,
                                                       id_number);
    while ((::EmbeddedProto::Error::NO_ERRORS == return_value) &&
           (::EmbeddedProto::Error::NO_ERRORS == tag_value)) {
      id_tag = static_cast<FieldNumber>(id_number);
      switch (id_tag) {
      case FieldNumber::REQUEST_ID:
        return_value = request_id_.deserialize_check_type(buffer, wire_type);
        break;

      case FieldNumber::POWER_ON:
      case FieldNumber::POWER_OFF:
      case FieldNumber::START_BREW:
      case FieldNumber::STOP_BREW:
      case FieldNumber::START_PUMP:
      case FieldNumber::STOP_PUMP:
      case FieldNumber::START_STEAM:
      case FieldNumber::STOP_STEAM:
      case FieldNumber::CONFIG:
        return_value = deserialize_command_oneof(id_tag, buffer, wire_type);
        break;

      case FieldNumber::NOT_SET:
        return_value = ::EmbeddedProto::Error::INVALID_FIELD_ID;
        break;

      default:
        return_value = skip_unknown_field(buffer, wire_type);
        break;
      }

      if (::EmbeddedProto::Error::NO_ERRORS == return_value) {
        // Read the next tag.
        tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(
            buffer, wire_type, id_number);
      }
    }

    // When an error was detect while reading the tag but no other errors where
    // found, set it in the return value.
    if ((::EmbeddedProto::Error::NO_ERRORS == return_value) &&
        (::EmbeddedProto::Error::NO_ERRORS != tag_value) &&
        (::EmbeddedProto::Error::END_OF_BUFFER !=
         tag_value)) // The end of the buffer is not an array in this case.
    {
      return_value = tag_value;
    }

    return return_value;
  };

  void clear() override {
    clear_request_id();
    clear_command_oneof();
  }

  static char const *field_number_to_name(const FieldNumber fieldNumber) {
    char const *name = nullptr;
    switch (fieldNumber) {
    case FieldNumber::REQUEST_ID:
      name = REQUEST_ID_NAME;
      break;
    case FieldNumber::POWER_ON:
      name = POWER_ON_NAME;
      break;
    case FieldNumber::POWER_OFF:
      name = POWER_OFF_NAME;
      break;
    case FieldNumber::START_BREW:
      name = START_BREW_NAME;
      break;
    case FieldNumber::STOP_BREW:
      name = STOP_BREW_NAME;
      break;
    case FieldNumber::START_PUMP:
      name = START_PUMP_NAME;
      break;
    case FieldNumber::STOP_PUMP:
      name = STOP_PUMP_NAME;
      break;
    case FieldNumber::START_STEAM:
      name = START_STEAM_NAME;
      break;
    case FieldNumber::STOP_STEAM:
      name = STOP_STEAM_NAME;
      break;
    case FieldNumber::CONFIG:
      name = CONFIG_NAME;
      break;
    default:
      name = "Invalid FieldNumber";
      break;
    }
    return name;
  }

#ifdef MSG_TO_STRING

  ::EmbeddedProto::string_view
  to_string(::EmbeddedProto::string_view &str) const {
    return this->to_string(str, 0, nullptr, true);
  }

  ::EmbeddedProto::string_view
  to_string(::EmbeddedProto::string_view &str, const uint32_t indent_level,
            char const *name, const bool first_field) const override {
    ::EmbeddedProto::string_view left_chars = str;
    int32_t n_chars_used = 0;

    if (!first_field) {
      // Add a comma behind the previous field.
      n_chars_used = snprintf(left_chars.data, left_chars.size, ",\n");
      if (0 < n_chars_used) {
        // Update the character pointer and characters left in the array.
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }
    }

    if (nullptr != name) {
      if (0 == indent_level) {
        n_chars_used =
            snprintf(left_chars.data, left_chars.size, "\"%s\": {\n", name);
      } else {
        n_chars_used = snprintf(left_chars.data, left_chars.size,
                                "%*s\"%s\": {\n", indent_level, " ", name);
      }
    } else {
      if (0 == indent_level) {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "{\n");
      } else {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s{\n",
                                indent_level, " ");
      }
    }

    if (0 < n_chars_used) {
      left_chars.data += n_chars_used;
      left_chars.size -= n_chars_used;
    }

    left_chars = request_id_.to_string(left_chars, indent_level + 2,
                                       REQUEST_ID_NAME, true);
    left_chars = to_string_command_oneof(left_chars, indent_level + 2, false);

    if (0 == indent_level) {
      n_chars_used = snprintf(left_chars.data, left_chars.size, "\n}");
    } else {
      n_chars_used = snprintf(left_chars.data, left_chars.size, "\n%*s}",
                              indent_level, " ");
    }

    if (0 < n_chars_used) {
      left_chars.data += n_chars_used;
      left_chars.size -= n_chars_used;
    }

    return left_chars;
  }

#endif // End of MSG_TO_STRING

private:
  ::EmbeddedProto::FieldBytes<Command_request_id_LENGTH> request_id_;

  FieldNumber which_command_oneof_ = FieldNumber::NOT_SET;
  union command_oneof {
    command_oneof() {}
    ~command_oneof() {}
    PowerOn power_on_;
    PowerOff power_off_;
    StartBrew start_brew_;
    StopBrew stop_brew_;
    StartPump start_pump_;
    StopPump stop_pump_;
    StartSteam start_steam_;
    StopSteam stop_steam_;
    Config config_;
  };
  command_oneof command_oneof_;

  void init_command_oneof(const FieldNumber field_id) {
    if (FieldNumber::NOT_SET != which_command_oneof_) {
      // First delete the old object in the oneof.
      clear_command_oneof();
    }

    // C++11 unions only support nontrivial members when you explicitly call the
    // placement new statement.
    switch (field_id) {
    case FieldNumber::POWER_ON:
      new (&command_oneof_.power_on_) PowerOn;
      break;

    case FieldNumber::POWER_OFF:
      new (&command_oneof_.power_off_) PowerOff;
      break;

    case FieldNumber::START_BREW:
      new (&command_oneof_.start_brew_) StartBrew;
      break;

    case FieldNumber::STOP_BREW:
      new (&command_oneof_.stop_brew_) StopBrew;
      break;

    case FieldNumber::START_PUMP:
      new (&command_oneof_.start_pump_) StartPump;
      break;

    case FieldNumber::STOP_PUMP:
      new (&command_oneof_.stop_pump_) StopPump;
      break;

    case FieldNumber::START_STEAM:
      new (&command_oneof_.start_steam_) StartSteam;
      break;

    case FieldNumber::STOP_STEAM:
      new (&command_oneof_.stop_steam_) StopSteam;
      break;

    case FieldNumber::CONFIG:
      new (&command_oneof_.config_) Config;
      break;

    default:
      break;
    }

    which_command_oneof_ = field_id;
  }

  void clear_command_oneof() {
    switch (which_command_oneof_) {
    case FieldNumber::POWER_ON:
      ::EmbeddedProto::destroy_at(&command_oneof_.power_on_);
      break;
    case FieldNumber::POWER_OFF:
      ::EmbeddedProto::destroy_at(&command_oneof_.power_off_);
      break;
    case FieldNumber::START_BREW:
      ::EmbeddedProto::destroy_at(&command_oneof_.start_brew_);
      break;
    case FieldNumber::STOP_BREW:
      ::EmbeddedProto::destroy_at(&command_oneof_.stop_brew_);
      break;
    case FieldNumber::START_PUMP:
      ::EmbeddedProto::destroy_at(&command_oneof_.start_pump_);
      break;
    case FieldNumber::STOP_PUMP:
      ::EmbeddedProto::destroy_at(&command_oneof_.stop_pump_);
      break;
    case FieldNumber::START_STEAM:
      ::EmbeddedProto::destroy_at(&command_oneof_.start_steam_);
      break;
    case FieldNumber::STOP_STEAM:
      ::EmbeddedProto::destroy_at(&command_oneof_.stop_steam_);
      break;
    case FieldNumber::CONFIG:
      ::EmbeddedProto::destroy_at(&command_oneof_.config_);
      break;
    default:
      break;
    }
    which_command_oneof_ = FieldNumber::NOT_SET;
  }

  ::EmbeddedProto::Error deserialize_command_oneof(
      const FieldNumber field_id, ::EmbeddedProto::ReadBufferInterface &buffer,
      const ::EmbeddedProto::WireFormatter::WireType wire_type) {
    ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;

    if (field_id != which_command_oneof_) {
      init_command_oneof(field_id);
    }

    switch (which_command_oneof_) {
    case FieldNumber::POWER_ON:
      return_value =
          command_oneof_.power_on_.deserialize_check_type(buffer, wire_type);
      break;
    case FieldNumber::POWER_OFF:
      return_value =
          command_oneof_.power_off_.deserialize_check_type(buffer, wire_type);
      break;
    case FieldNumber::START_BREW:
      return_value =
          command_oneof_.start_brew_.deserialize_check_type(buffer, wire_type);
      break;
    case FieldNumber::STOP_BREW:
      return_value =
          command_oneof_.stop_brew_.deserialize_check_type(buffer, wire_type);
      break;
    case FieldNumber::START_PUMP:
      return_value =
          command_oneof_.start_pump_.deserialize_check_type(buffer, wire_type);
      break;
    case FieldNumber::STOP_PUMP:
      return_value =
          command_oneof_.stop_pump_.deserialize_check_type(buffer, wire_type);
      break;
    case FieldNumber::START_STEAM:
      return_value =
          command_oneof_.start_steam_.deserialize_check_type(buffer, wire_type);
      break;
    case FieldNumber::STOP_STEAM:
      return_value =
          command_oneof_.stop_steam_.deserialize_check_type(buffer, wire_type);
      break;
    case FieldNumber::CONFIG:
      return_value =
          command_oneof_.config_.deserialize_check_type(buffer, wire_type);
      break;
    default:
      break;
    }

    if (::EmbeddedProto::Error::NO_ERRORS != return_value) {
      clear_command_oneof();
    }
    return return_value;
  }

#ifdef MSG_TO_STRING
  ::EmbeddedProto::string_view
  to_string_command_oneof(::EmbeddedProto::string_view &str,
                          const uint32_t indent_level,
                          const bool first_field) const {
    ::EmbeddedProto::string_view left_chars = str;

    switch (which_command_oneof_) {
    case FieldNumber::POWER_ON:
      left_chars = command_oneof_.power_on_.to_string(
          left_chars, indent_level, POWER_ON_NAME, first_field);
      break;
    case FieldNumber::POWER_OFF:
      left_chars = command_oneof_.power_off_.to_string(
          left_chars, indent_level, POWER_OFF_NAME, first_field);
      break;
    case FieldNumber::START_BREW:
      left_chars = command_oneof_.start_brew_.to_string(
          left_chars, indent_level, START_BREW_NAME, first_field);
      break;
    case FieldNumber::STOP_BREW:
      left_chars = command_oneof_.stop_brew_.to_string(
          left_chars, indent_level, STOP_BREW_NAME, first_field);
      break;
    case FieldNumber::START_PUMP:
      left_chars = command_oneof_.start_pump_.to_string(
          left_chars, indent_level, START_PUMP_NAME, first_field);
      break;
    case FieldNumber::STOP_PUMP:
      left_chars = command_oneof_.stop_pump_.to_string(
          left_chars, indent_level, STOP_PUMP_NAME, first_field);
      break;
    case FieldNumber::START_STEAM:
      left_chars = command_oneof_.start_steam_.to_string(
          left_chars, indent_level, START_STEAM_NAME, first_field);
      break;
    case FieldNumber::STOP_STEAM:
      left_chars = command_oneof_.stop_steam_.to_string(
          left_chars, indent_level, STOP_STEAM_NAME, first_field);
      break;
    case FieldNumber::CONFIG:
      left_chars = command_oneof_.config_.to_string(left_chars, indent_level,
                                                    CONFIG_NAME, first_field);
      break;
    default:
      break;
    }

    return left_chars;
  }

#endif // End of MSG_TO_STRING
};

template <uint32_t StateUpdate_boilerTemp_FloatSensorReading_error_LENGTH,
          uint32_t StateUpdate_pressure_FloatSensorReading_error_LENGTH>
class StateUpdate final : public ::EmbeddedProto::MessageInterface {
public:
  StateUpdate() = default;
  StateUpdate(const StateUpdate &rhs) {
    set_is_on(rhs.get_is_on());
    set_is_brewing(rhs.get_is_brewing());
    set_is_pumping(rhs.get_is_pumping());
    set_is_steaming(rhs.get_is_steaming());
    set_boilerTemp(rhs.get_boilerTemp());
    set_pressure(rhs.get_pressure());
  }

  StateUpdate(const StateUpdate &&rhs) noexcept {
    set_is_on(rhs.get_is_on());
    set_is_brewing(rhs.get_is_brewing());
    set_is_pumping(rhs.get_is_pumping());
    set_is_steaming(rhs.get_is_steaming());
    set_boilerTemp(rhs.get_boilerTemp());
    set_pressure(rhs.get_pressure());
  }

  ~StateUpdate() override = default;

  enum class FieldNumber : uint32_t {
    NOT_SET = 0,
    IS_ON = 1,
    IS_BREWING = 2,
    IS_PUMPING = 3,
    IS_STEAMING = 4,
    BOILERTEMP = 5,
    PRESSURE = 6
  };

  StateUpdate &operator=(const StateUpdate &rhs) {
    set_is_on(rhs.get_is_on());
    set_is_brewing(rhs.get_is_brewing());
    set_is_pumping(rhs.get_is_pumping());
    set_is_steaming(rhs.get_is_steaming());
    set_boilerTemp(rhs.get_boilerTemp());
    set_pressure(rhs.get_pressure());
    return *this;
  }

  StateUpdate &operator=(const StateUpdate &&rhs) noexcept {
    set_is_on(rhs.get_is_on());
    set_is_brewing(rhs.get_is_brewing());
    set_is_pumping(rhs.get_is_pumping());
    set_is_steaming(rhs.get_is_steaming());
    set_boilerTemp(rhs.get_boilerTemp());
    set_pressure(rhs.get_pressure());
    return *this;
  }

  static constexpr char const *IS_ON_NAME = "is_on";
  inline void clear_is_on() { is_on_.clear(); }
  inline void set_is_on(const bool &value) { is_on_ = value; }
  inline void set_is_on(const bool &&value) { is_on_ = value; }
  inline bool &mutable_is_on() { return is_on_.get(); }
  inline const bool &get_is_on() const { return is_on_.get(); }
  inline bool is_on() const { return is_on_.get(); }

  static constexpr char const *IS_BREWING_NAME = "is_brewing";
  inline void clear_is_brewing() { is_brewing_.clear(); }
  inline void set_is_brewing(const bool &value) { is_brewing_ = value; }
  inline void set_is_brewing(const bool &&value) { is_brewing_ = value; }
  inline bool &mutable_is_brewing() { return is_brewing_.get(); }
  inline const bool &get_is_brewing() const { return is_brewing_.get(); }
  inline bool is_brewing() const { return is_brewing_.get(); }

  static constexpr char const *IS_PUMPING_NAME = "is_pumping";
  inline void clear_is_pumping() { is_pumping_.clear(); }
  inline void set_is_pumping(const bool &value) { is_pumping_ = value; }
  inline void set_is_pumping(const bool &&value) { is_pumping_ = value; }
  inline bool &mutable_is_pumping() { return is_pumping_.get(); }
  inline const bool &get_is_pumping() const { return is_pumping_.get(); }
  inline bool is_pumping() const { return is_pumping_.get(); }

  static constexpr char const *IS_STEAMING_NAME = "is_steaming";
  inline void clear_is_steaming() { is_steaming_.clear(); }
  inline void set_is_steaming(const bool &value) { is_steaming_ = value; }
  inline void set_is_steaming(const bool &&value) { is_steaming_ = value; }
  inline bool &mutable_is_steaming() { return is_steaming_.get(); }
  inline const bool &get_is_steaming() const { return is_steaming_.get(); }
  inline bool is_steaming() const { return is_steaming_.get(); }

  static constexpr char const *BOILERTEMP_NAME = "boilerTemp";
  inline void clear_boilerTemp() { boilerTemp_.clear(); }
  inline void set_boilerTemp(
      const FloatSensorReading<
          StateUpdate_boilerTemp_FloatSensorReading_error_LENGTH> &value) {
    boilerTemp_ = value;
  }
  inline void set_boilerTemp(
      const FloatSensorReading<
          StateUpdate_boilerTemp_FloatSensorReading_error_LENGTH> &&value) {
    boilerTemp_ = value;
  }
  inline FloatSensorReading<
      StateUpdate_boilerTemp_FloatSensorReading_error_LENGTH> &
  mutable_boilerTemp() {
    return boilerTemp_;
  }
  inline const FloatSensorReading<
      StateUpdate_boilerTemp_FloatSensorReading_error_LENGTH> &
  get_boilerTemp() const {
    return boilerTemp_;
  }
  inline const FloatSensorReading<
      StateUpdate_boilerTemp_FloatSensorReading_error_LENGTH> &
  boilerTemp() const {
    return boilerTemp_;
  }

  static constexpr char const *PRESSURE_NAME = "pressure";
  inline void clear_pressure() { pressure_.clear(); }
  inline void
  set_pressure(const FloatSensorReading<
               StateUpdate_pressure_FloatSensorReading_error_LENGTH> &value) {
    pressure_ = value;
  }
  inline void
  set_pressure(const FloatSensorReading<
               StateUpdate_pressure_FloatSensorReading_error_LENGTH> &&value) {
    pressure_ = value;
  }
  inline FloatSensorReading<
      StateUpdate_pressure_FloatSensorReading_error_LENGTH> &
  mutable_pressure() {
    return pressure_;
  }
  inline const FloatSensorReading<
      StateUpdate_pressure_FloatSensorReading_error_LENGTH> &
  get_pressure() const {
    return pressure_;
  }
  inline const FloatSensorReading<
      StateUpdate_pressure_FloatSensorReading_error_LENGTH> &
  pressure() const {
    return pressure_;
  }

  ::EmbeddedProto::Error
  serialize(::EmbeddedProto::WriteBufferInterface &buffer) const override {
    ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;

    if ((false != is_on_.get()) &&
        (::EmbeddedProto::Error::NO_ERRORS == return_value)) {
      return_value = is_on_.serialize_with_id(
          static_cast<uint32_t>(FieldNumber::IS_ON), buffer, false);
    }

    if ((false != is_brewing_.get()) &&
        (::EmbeddedProto::Error::NO_ERRORS == return_value)) {
      return_value = is_brewing_.serialize_with_id(
          static_cast<uint32_t>(FieldNumber::IS_BREWING), buffer, false);
    }

    if ((false != is_pumping_.get()) &&
        (::EmbeddedProto::Error::NO_ERRORS == return_value)) {
      return_value = is_pumping_.serialize_with_id(
          static_cast<uint32_t>(FieldNumber::IS_PUMPING), buffer, false);
    }

    if ((false != is_steaming_.get()) &&
        (::EmbeddedProto::Error::NO_ERRORS == return_value)) {
      return_value = is_steaming_.serialize_with_id(
          static_cast<uint32_t>(FieldNumber::IS_STEAMING), buffer, false);
    }

    if (::EmbeddedProto::Error::NO_ERRORS == return_value) {
      return_value = boilerTemp_.serialize_with_id(
          static_cast<uint32_t>(FieldNumber::BOILERTEMP), buffer, false);
    }

    if (::EmbeddedProto::Error::NO_ERRORS == return_value) {
      return_value = pressure_.serialize_with_id(
          static_cast<uint32_t>(FieldNumber::PRESSURE), buffer, false);
    }

    return return_value;
  };

  ::EmbeddedProto::Error
  deserialize(::EmbeddedProto::ReadBufferInterface &buffer) override {
    ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;
    ::EmbeddedProto::WireFormatter::WireType wire_type =
        ::EmbeddedProto::WireFormatter::WireType::VARINT;
    uint32_t id_number = 0;
    FieldNumber id_tag = FieldNumber::NOT_SET;

    ::EmbeddedProto::Error tag_value =
        ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type,
                                                       id_number);
    while ((::EmbeddedProto::Error::NO_ERRORS == return_value) &&
           (::EmbeddedProto::Error::NO_ERRORS == tag_value)) {
      id_tag = static_cast<FieldNumber>(id_number);
      switch (id_tag) {
      case FieldNumber::IS_ON:
        return_value = is_on_.deserialize_check_type(buffer, wire_type);
        break;

      case FieldNumber::IS_BREWING:
        return_value = is_brewing_.deserialize_check_type(buffer, wire_type);
        break;

      case FieldNumber::IS_PUMPING:
        return_value = is_pumping_.deserialize_check_type(buffer, wire_type);
        break;

      case FieldNumber::IS_STEAMING:
        return_value = is_steaming_.deserialize_check_type(buffer, wire_type);
        break;

      case FieldNumber::BOILERTEMP:
        return_value = boilerTemp_.deserialize_check_type(buffer, wire_type);
        break;

      case FieldNumber::PRESSURE:
        return_value = pressure_.deserialize_check_type(buffer, wire_type);
        break;

      case FieldNumber::NOT_SET:
        return_value = ::EmbeddedProto::Error::INVALID_FIELD_ID;
        break;

      default:
        return_value = skip_unknown_field(buffer, wire_type);
        break;
      }

      if (::EmbeddedProto::Error::NO_ERRORS == return_value) {
        // Read the next tag.
        tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(
            buffer, wire_type, id_number);
      }
    }

    // When an error was detect while reading the tag but no other errors where
    // found, set it in the return value.
    if ((::EmbeddedProto::Error::NO_ERRORS == return_value) &&
        (::EmbeddedProto::Error::NO_ERRORS != tag_value) &&
        (::EmbeddedProto::Error::END_OF_BUFFER !=
         tag_value)) // The end of the buffer is not an array in this case.
    {
      return_value = tag_value;
    }

    return return_value;
  };

  void clear() override {
    clear_is_on();
    clear_is_brewing();
    clear_is_pumping();
    clear_is_steaming();
    clear_boilerTemp();
    clear_pressure();
  }

  static char const *field_number_to_name(const FieldNumber fieldNumber) {
    char const *name = nullptr;
    switch (fieldNumber) {
    case FieldNumber::IS_ON:
      name = IS_ON_NAME;
      break;
    case FieldNumber::IS_BREWING:
      name = IS_BREWING_NAME;
      break;
    case FieldNumber::IS_PUMPING:
      name = IS_PUMPING_NAME;
      break;
    case FieldNumber::IS_STEAMING:
      name = IS_STEAMING_NAME;
      break;
    case FieldNumber::BOILERTEMP:
      name = BOILERTEMP_NAME;
      break;
    case FieldNumber::PRESSURE:
      name = PRESSURE_NAME;
      break;
    default:
      name = "Invalid FieldNumber";
      break;
    }
    return name;
  }

#ifdef MSG_TO_STRING

  ::EmbeddedProto::string_view
  to_string(::EmbeddedProto::string_view &str) const {
    return this->to_string(str, 0, nullptr, true);
  }

  ::EmbeddedProto::string_view
  to_string(::EmbeddedProto::string_view &str, const uint32_t indent_level,
            char const *name, const bool first_field) const override {
    ::EmbeddedProto::string_view left_chars = str;
    int32_t n_chars_used = 0;

    if (!first_field) {
      // Add a comma behind the previous field.
      n_chars_used = snprintf(left_chars.data, left_chars.size, ",\n");
      if (0 < n_chars_used) {
        // Update the character pointer and characters left in the array.
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }
    }

    if (nullptr != name) {
      if (0 == indent_level) {
        n_chars_used =
            snprintf(left_chars.data, left_chars.size, "\"%s\": {\n", name);
      } else {
        n_chars_used = snprintf(left_chars.data, left_chars.size,
                                "%*s\"%s\": {\n", indent_level, " ", name);
      }
    } else {
      if (0 == indent_level) {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "{\n");
      } else {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s{\n",
                                indent_level, " ");
      }
    }

    if (0 < n_chars_used) {
      left_chars.data += n_chars_used;
      left_chars.size -= n_chars_used;
    }

    left_chars =
        is_on_.to_string(left_chars, indent_level + 2, IS_ON_NAME, true);
    left_chars = is_brewing_.to_string(left_chars, indent_level + 2,
                                       IS_BREWING_NAME, false);
    left_chars = is_pumping_.to_string(left_chars, indent_level + 2,
                                       IS_PUMPING_NAME, false);
    left_chars = is_steaming_.to_string(left_chars, indent_level + 2,
                                        IS_STEAMING_NAME, false);
    left_chars = boilerTemp_.to_string(left_chars, indent_level + 2,
                                       BOILERTEMP_NAME, false);
    left_chars =
        pressure_.to_string(left_chars, indent_level + 2, PRESSURE_NAME, false);

    if (0 == indent_level) {
      n_chars_used = snprintf(left_chars.data, left_chars.size, "\n}");
    } else {
      n_chars_used = snprintf(left_chars.data, left_chars.size, "\n%*s}",
                              indent_level, " ");
    }

    if (0 < n_chars_used) {
      left_chars.data += n_chars_used;
      left_chars.size -= n_chars_used;
    }

    return left_chars;
  }

#endif // End of MSG_TO_STRING

private:
  EmbeddedProto::boolean is_on_ = false;
  EmbeddedProto::boolean is_brewing_ = false;
  EmbeddedProto::boolean is_pumping_ = false;
  EmbeddedProto::boolean is_steaming_ = false;
  FloatSensorReading<StateUpdate_boilerTemp_FloatSensorReading_error_LENGTH>
      boilerTemp_;
  FloatSensorReading<StateUpdate_pressure_FloatSensorReading_error_LENGTH>
      pressure_;
};

template <
    uint32_t Event_request_id_LENGTH,
    uint32_t
        Event_state_update_StateUpdate_boilerTemp_FloatSensorReading_error_LENGTH,
    uint32_t
        Event_state_update_StateUpdate_pressure_FloatSensorReading_error_LENGTH,
    uint32_t Event_log_LogMessage_msg_LENGTH>
class Event final : public ::EmbeddedProto::MessageInterface {
public:
  Event() = default;
  Event(const Event &rhs) {
    set_request_id(rhs.get_request_id());
    if (rhs.get_which_event_oneof() != which_event_oneof_) {
      // First delete the old object in the oneof.
      clear_event_oneof();
    }

    switch (rhs.get_which_event_oneof()) {
    case FieldNumber::STATE_UPDATE:
      set_state_update(rhs.get_state_update());
      break;

    case FieldNumber::CONFIG:
      set_config(rhs.get_config());
      break;

    case FieldNumber::LOG:
      set_log(rhs.get_log());
      break;

    default:
      break;
    }
  }

  Event(const Event &&rhs) noexcept {
    set_request_id(rhs.get_request_id());
    if (rhs.get_which_event_oneof() != which_event_oneof_) {
      // First delete the old object in the oneof.
      clear_event_oneof();
    }

    switch (rhs.get_which_event_oneof()) {
    case FieldNumber::STATE_UPDATE:
      set_state_update(rhs.get_state_update());
      break;

    case FieldNumber::CONFIG:
      set_config(rhs.get_config());
      break;

    case FieldNumber::LOG:
      set_log(rhs.get_log());
      break;

    default:
      break;
    }
  }

  ~Event() override = default;

  enum class FieldNumber : uint32_t {
    NOT_SET = 0,
    REQUEST_ID = 1,
    STATE_UPDATE = 2,
    CONFIG = 3,
    LOG = 4
  };

  Event &operator=(const Event &rhs) {
    set_request_id(rhs.get_request_id());
    if (rhs.get_which_event_oneof() != which_event_oneof_) {
      // First delete the old object in the oneof.
      clear_event_oneof();
    }

    switch (rhs.get_which_event_oneof()) {
    case FieldNumber::STATE_UPDATE:
      set_state_update(rhs.get_state_update());
      break;

    case FieldNumber::CONFIG:
      set_config(rhs.get_config());
      break;

    case FieldNumber::LOG:
      set_log(rhs.get_log());
      break;

    default:
      break;
    }

    return *this;
  }

  Event &operator=(const Event &&rhs) noexcept {
    set_request_id(rhs.get_request_id());
    if (rhs.get_which_event_oneof() != which_event_oneof_) {
      // First delete the old object in the oneof.
      clear_event_oneof();
    }

    switch (rhs.get_which_event_oneof()) {
    case FieldNumber::STATE_UPDATE:
      set_state_update(rhs.get_state_update());
      break;

    case FieldNumber::CONFIG:
      set_config(rhs.get_config());
      break;

    case FieldNumber::LOG:
      set_log(rhs.get_log());
      break;

    default:
      break;
    }

    return *this;
  }

  static constexpr char const *REQUEST_ID_NAME = "request_id";
  inline void clear_request_id() { request_id_.clear(); }
  inline ::EmbeddedProto::FieldBytes<Event_request_id_LENGTH> &
  mutable_request_id() {
    return request_id_;
  }
  inline void set_request_id(
      const ::EmbeddedProto::FieldBytes<Event_request_id_LENGTH> &rhs) {
    request_id_.set(rhs);
  }
  inline const ::EmbeddedProto::FieldBytes<Event_request_id_LENGTH> &
  get_request_id() const {
    return request_id_;
  }
  inline const uint8_t *request_id() const { return request_id_.get_const(); }

  FieldNumber get_which_event_oneof() const { return which_event_oneof_; }

  static constexpr char const *STATE_UPDATE_NAME = "state_update";
  inline bool has_state_update() const {
    return FieldNumber::STATE_UPDATE == which_event_oneof_;
  }
  inline void clear_state_update() {
    if (FieldNumber::STATE_UPDATE == which_event_oneof_) {
      which_event_oneof_ = FieldNumber::NOT_SET;
      event_oneof_.state_update_.~StateUpdate<
          Event_state_update_StateUpdate_boilerTemp_FloatSensorReading_error_LENGTH,
          Event_state_update_StateUpdate_pressure_FloatSensorReading_error_LENGTH>();
    }
  }
  inline void set_state_update(
      const StateUpdate<
          Event_state_update_StateUpdate_boilerTemp_FloatSensorReading_error_LENGTH,
          Event_state_update_StateUpdate_pressure_FloatSensorReading_error_LENGTH>
          &value) {
    if (FieldNumber::STATE_UPDATE != which_event_oneof_) {
      init_event_oneof(FieldNumber::STATE_UPDATE);
    }
    event_oneof_.state_update_ = value;
  }
  inline void set_state_update(
      const StateUpdate<
          Event_state_update_StateUpdate_boilerTemp_FloatSensorReading_error_LENGTH,
          Event_state_update_StateUpdate_pressure_FloatSensorReading_error_LENGTH>
          &&value) {
    if (FieldNumber::STATE_UPDATE != which_event_oneof_) {
      init_event_oneof(FieldNumber::STATE_UPDATE);
    }
    event_oneof_.state_update_ = value;
  }
  inline StateUpdate<
      Event_state_update_StateUpdate_boilerTemp_FloatSensorReading_error_LENGTH,
      Event_state_update_StateUpdate_pressure_FloatSensorReading_error_LENGTH> &
  mutable_state_update() {
    if (FieldNumber::STATE_UPDATE != which_event_oneof_) {
      init_event_oneof(FieldNumber::STATE_UPDATE);
    }
    return event_oneof_.state_update_;
  }
  inline const StateUpdate<
      Event_state_update_StateUpdate_boilerTemp_FloatSensorReading_error_LENGTH,
      Event_state_update_StateUpdate_pressure_FloatSensorReading_error_LENGTH> &
  get_state_update() const {
    return event_oneof_.state_update_;
  }
  inline const StateUpdate<
      Event_state_update_StateUpdate_boilerTemp_FloatSensorReading_error_LENGTH,
      Event_state_update_StateUpdate_pressure_FloatSensorReading_error_LENGTH> &
  state_update() const {
    return event_oneof_.state_update_;
  }

  static constexpr char const *CONFIG_NAME = "config";
  inline bool has_config() const {
    return FieldNumber::CONFIG == which_event_oneof_;
  }
  inline void clear_config() {
    if (FieldNumber::CONFIG == which_event_oneof_) {
      which_event_oneof_ = FieldNumber::NOT_SET;
      event_oneof_.config_.~Config();
    }
  }
  inline void set_config(const Config &value) {
    if (FieldNumber::CONFIG != which_event_oneof_) {
      init_event_oneof(FieldNumber::CONFIG);
    }
    event_oneof_.config_ = value;
  }
  inline void set_config(const Config &&value) {
    if (FieldNumber::CONFIG != which_event_oneof_) {
      init_event_oneof(FieldNumber::CONFIG);
    }
    event_oneof_.config_ = value;
  }
  inline Config &mutable_config() {
    if (FieldNumber::CONFIG != which_event_oneof_) {
      init_event_oneof(FieldNumber::CONFIG);
    }
    return event_oneof_.config_;
  }
  inline const Config &get_config() const { return event_oneof_.config_; }
  inline const Config &config() const { return event_oneof_.config_; }

  static constexpr char const *LOG_NAME = "log";
  inline bool has_log() const { return FieldNumber::LOG == which_event_oneof_; }
  inline void clear_log() {
    if (FieldNumber::LOG == which_event_oneof_) {
      which_event_oneof_ = FieldNumber::NOT_SET;
      event_oneof_.log_.~LogMessage<Event_log_LogMessage_msg_LENGTH>();
    }
  }
  inline void
  set_log(const LogMessage<Event_log_LogMessage_msg_LENGTH> &value) {
    if (FieldNumber::LOG != which_event_oneof_) {
      init_event_oneof(FieldNumber::LOG);
    }
    event_oneof_.log_ = value;
  }
  inline void
  set_log(const LogMessage<Event_log_LogMessage_msg_LENGTH> &&value) {
    if (FieldNumber::LOG != which_event_oneof_) {
      init_event_oneof(FieldNumber::LOG);
    }
    event_oneof_.log_ = value;
  }
  inline LogMessage<Event_log_LogMessage_msg_LENGTH> &mutable_log() {
    if (FieldNumber::LOG != which_event_oneof_) {
      init_event_oneof(FieldNumber::LOG);
    }
    return event_oneof_.log_;
  }
  inline const LogMessage<Event_log_LogMessage_msg_LENGTH> &get_log() const {
    return event_oneof_.log_;
  }
  inline const LogMessage<Event_log_LogMessage_msg_LENGTH> &log() const {
    return event_oneof_.log_;
  }

  ::EmbeddedProto::Error
  serialize(::EmbeddedProto::WriteBufferInterface &buffer) const override {
    ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;

    if (::EmbeddedProto::Error::NO_ERRORS == return_value) {
      return_value = request_id_.serialize_with_id(
          static_cast<uint32_t>(FieldNumber::REQUEST_ID), buffer, false);
    }

    switch (which_event_oneof_) {
    case FieldNumber::STATE_UPDATE:
      if (has_state_update() &&
          (::EmbeddedProto::Error::NO_ERRORS == return_value)) {
        return_value = event_oneof_.state_update_.serialize_with_id(
            static_cast<uint32_t>(FieldNumber::STATE_UPDATE), buffer, true);
      }
      break;

    case FieldNumber::CONFIG:
      if (has_config() && (::EmbeddedProto::Error::NO_ERRORS == return_value)) {
        return_value = event_oneof_.config_.serialize_with_id(
            static_cast<uint32_t>(FieldNumber::CONFIG), buffer, true);
      }
      break;

    case FieldNumber::LOG:
      if (has_log() && (::EmbeddedProto::Error::NO_ERRORS == return_value)) {
        return_value = event_oneof_.log_.serialize_with_id(
            static_cast<uint32_t>(FieldNumber::LOG), buffer, true);
      }
      break;

    default:
      break;
    }

    return return_value;
  };

  ::EmbeddedProto::Error
  deserialize(::EmbeddedProto::ReadBufferInterface &buffer) override {
    ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;
    ::EmbeddedProto::WireFormatter::WireType wire_type =
        ::EmbeddedProto::WireFormatter::WireType::VARINT;
    uint32_t id_number = 0;
    FieldNumber id_tag = FieldNumber::NOT_SET;

    ::EmbeddedProto::Error tag_value =
        ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type,
                                                       id_number);
    while ((::EmbeddedProto::Error::NO_ERRORS == return_value) &&
           (::EmbeddedProto::Error::NO_ERRORS == tag_value)) {
      id_tag = static_cast<FieldNumber>(id_number);
      switch (id_tag) {
      case FieldNumber::REQUEST_ID:
        return_value = request_id_.deserialize_check_type(buffer, wire_type);
        break;

      case FieldNumber::STATE_UPDATE:
      case FieldNumber::CONFIG:
      case FieldNumber::LOG:
        return_value = deserialize_event_oneof(id_tag, buffer, wire_type);
        break;

      case FieldNumber::NOT_SET:
        return_value = ::EmbeddedProto::Error::INVALID_FIELD_ID;
        break;

      default:
        return_value = skip_unknown_field(buffer, wire_type);
        break;
      }

      if (::EmbeddedProto::Error::NO_ERRORS == return_value) {
        // Read the next tag.
        tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(
            buffer, wire_type, id_number);
      }
    }

    // When an error was detect while reading the tag but no other errors where
    // found, set it in the return value.
    if ((::EmbeddedProto::Error::NO_ERRORS == return_value) &&
        (::EmbeddedProto::Error::NO_ERRORS != tag_value) &&
        (::EmbeddedProto::Error::END_OF_BUFFER !=
         tag_value)) // The end of the buffer is not an array in this case.
    {
      return_value = tag_value;
    }

    return return_value;
  };

  void clear() override {
    clear_request_id();
    clear_event_oneof();
  }

  static char const *field_number_to_name(const FieldNumber fieldNumber) {
    char const *name = nullptr;
    switch (fieldNumber) {
    case FieldNumber::REQUEST_ID:
      name = REQUEST_ID_NAME;
      break;
    case FieldNumber::STATE_UPDATE:
      name = STATE_UPDATE_NAME;
      break;
    case FieldNumber::CONFIG:
      name = CONFIG_NAME;
      break;
    case FieldNumber::LOG:
      name = LOG_NAME;
      break;
    default:
      name = "Invalid FieldNumber";
      break;
    }
    return name;
  }

#ifdef MSG_TO_STRING

  ::EmbeddedProto::string_view
  to_string(::EmbeddedProto::string_view &str) const {
    return this->to_string(str, 0, nullptr, true);
  }

  ::EmbeddedProto::string_view
  to_string(::EmbeddedProto::string_view &str, const uint32_t indent_level,
            char const *name, const bool first_field) const override {
    ::EmbeddedProto::string_view left_chars = str;
    int32_t n_chars_used = 0;

    if (!first_field) {
      // Add a comma behind the previous field.
      n_chars_used = snprintf(left_chars.data, left_chars.size, ",\n");
      if (0 < n_chars_used) {
        // Update the character pointer and characters left in the array.
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }
    }

    if (nullptr != name) {
      if (0 == indent_level) {
        n_chars_used =
            snprintf(left_chars.data, left_chars.size, "\"%s\": {\n", name);
      } else {
        n_chars_used = snprintf(left_chars.data, left_chars.size,
                                "%*s\"%s\": {\n", indent_level, " ", name);
      }
    } else {
      if (0 == indent_level) {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "{\n");
      } else {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s{\n",
                                indent_level, " ");
      }
    }

    if (0 < n_chars_used) {
      left_chars.data += n_chars_used;
      left_chars.size -= n_chars_used;
    }

    left_chars = request_id_.to_string(left_chars, indent_level + 2,
                                       REQUEST_ID_NAME, true);
    left_chars = to_string_event_oneof(left_chars, indent_level + 2, false);

    if (0 == indent_level) {
      n_chars_used = snprintf(left_chars.data, left_chars.size, "\n}");
    } else {
      n_chars_used = snprintf(left_chars.data, left_chars.size, "\n%*s}",
                              indent_level, " ");
    }

    if (0 < n_chars_used) {
      left_chars.data += n_chars_used;
      left_chars.size -= n_chars_used;
    }

    return left_chars;
  }

#endif // End of MSG_TO_STRING

private:
  ::EmbeddedProto::FieldBytes<Event_request_id_LENGTH> request_id_;

  FieldNumber which_event_oneof_ = FieldNumber::NOT_SET;
  union event_oneof {
    event_oneof() {}
    ~event_oneof() {}
    StateUpdate<
        Event_state_update_StateUpdate_boilerTemp_FloatSensorReading_error_LENGTH,
        Event_state_update_StateUpdate_pressure_FloatSensorReading_error_LENGTH>
        state_update_;
    Config config_;
    LogMessage<Event_log_LogMessage_msg_LENGTH> log_;
  };
  event_oneof event_oneof_;

  void init_event_oneof(const FieldNumber field_id) {
    if (FieldNumber::NOT_SET != which_event_oneof_) {
      // First delete the old object in the oneof.
      clear_event_oneof();
    }

    // C++11 unions only support nontrivial members when you explicitly call the
    // placement new statement.
    switch (field_id) {
    case FieldNumber::STATE_UPDATE:
      new (&event_oneof_.state_update_) StateUpdate<
          Event_state_update_StateUpdate_boilerTemp_FloatSensorReading_error_LENGTH,
          Event_state_update_StateUpdate_pressure_FloatSensorReading_error_LENGTH>;
      break;

    case FieldNumber::CONFIG:
      new (&event_oneof_.config_) Config;
      break;

    case FieldNumber::LOG:
      new (&event_oneof_.log_) LogMessage<Event_log_LogMessage_msg_LENGTH>;
      break;

    default:
      break;
    }

    which_event_oneof_ = field_id;
  }

  void clear_event_oneof() {
    switch (which_event_oneof_) {
    case FieldNumber::STATE_UPDATE:
      ::EmbeddedProto::destroy_at(&event_oneof_.state_update_);
      break;
    case FieldNumber::CONFIG:
      ::EmbeddedProto::destroy_at(&event_oneof_.config_);
      break;
    case FieldNumber::LOG:
      ::EmbeddedProto::destroy_at(&event_oneof_.log_);
      break;
    default:
      break;
    }
    which_event_oneof_ = FieldNumber::NOT_SET;
  }

  ::EmbeddedProto::Error deserialize_event_oneof(
      const FieldNumber field_id, ::EmbeddedProto::ReadBufferInterface &buffer,
      const ::EmbeddedProto::WireFormatter::WireType wire_type) {
    ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;

    if (field_id != which_event_oneof_) {
      init_event_oneof(field_id);
    }

    switch (which_event_oneof_) {
    case FieldNumber::STATE_UPDATE:
      return_value =
          event_oneof_.state_update_.deserialize_check_type(buffer, wire_type);
      break;
    case FieldNumber::CONFIG:
      return_value =
          event_oneof_.config_.deserialize_check_type(buffer, wire_type);
      break;
    case FieldNumber::LOG:
      return_value =
          event_oneof_.log_.deserialize_check_type(buffer, wire_type);
      break;
    default:
      break;
    }

    if (::EmbeddedProto::Error::NO_ERRORS != return_value) {
      clear_event_oneof();
    }
    return return_value;
  }

#ifdef MSG_TO_STRING
  ::EmbeddedProto::string_view
  to_string_event_oneof(::EmbeddedProto::string_view &str,
                        const uint32_t indent_level,
                        const bool first_field) const {
    ::EmbeddedProto::string_view left_chars = str;

    switch (which_event_oneof_) {
    case FieldNumber::STATE_UPDATE:
      left_chars = event_oneof_.state_update_.to_string(
          left_chars, indent_level, STATE_UPDATE_NAME, first_field);
      break;
    case FieldNumber::CONFIG:
      left_chars = event_oneof_.config_.to_string(left_chars, indent_level,
                                                  CONFIG_NAME, first_field);
      break;
    case FieldNumber::LOG:
      left_chars = event_oneof_.log_.to_string(left_chars, indent_level,
                                               LOG_NAME, first_field);
      break;
    default:
      break;
    }

    return left_chars;
  }

#endif // End of MSG_TO_STRING
};

#endif // API_H