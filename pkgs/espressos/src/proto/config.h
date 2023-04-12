// This file is generated. Please do not edit!
#ifndef CONFIG_H
#define CONFIG_H

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

class PIDTunings final : public ::EmbeddedProto::MessageInterface {
public:
  PIDTunings() = default;
  PIDTunings(const PIDTunings &rhs) {
    set_P(rhs.get_P());
    set_I(rhs.get_I());
    set_D(rhs.get_D());
  }

  PIDTunings(const PIDTunings &&rhs) noexcept {
    set_P(rhs.get_P());
    set_I(rhs.get_I());
    set_D(rhs.get_D());
  }

  ~PIDTunings() override = default;

  enum class FieldNumber : uint32_t { NOT_SET = 0, P = 1, I = 2, D = 3 };

  PIDTunings &operator=(const PIDTunings &rhs) {
    set_P(rhs.get_P());
    set_I(rhs.get_I());
    set_D(rhs.get_D());
    return *this;
  }

  PIDTunings &operator=(const PIDTunings &&rhs) noexcept {
    set_P(rhs.get_P());
    set_I(rhs.get_I());
    set_D(rhs.get_D());
    return *this;
  }

  static constexpr char const *P_NAME = "P";
  inline void clear_P() { P_.clear(); }
  inline void set_P(const double &value) { P_ = value; }
  inline void set_P(const double &&value) { P_ = value; }
  inline double &mutable_P() { return P_.get(); }
  inline const double &get_P() const { return P_.get(); }
  inline double P() const { return P_.get(); }

  static constexpr char const *I_NAME = "I";
  inline void clear_I() { I_.clear(); }
  inline void set_I(const double &value) { I_ = value; }
  inline void set_I(const double &&value) { I_ = value; }
  inline double &mutable_I() { return I_.get(); }
  inline const double &get_I() const { return I_.get(); }
  inline double I() const { return I_.get(); }

  static constexpr char const *D_NAME = "D";
  inline void clear_D() { D_.clear(); }
  inline void set_D(const double &value) { D_ = value; }
  inline void set_D(const double &&value) { D_ = value; }
  inline double &mutable_D() { return D_.get(); }
  inline const double &get_D() const { return D_.get(); }
  inline double D() const { return D_.get(); }

  ::EmbeddedProto::Error
  serialize(::EmbeddedProto::WriteBufferInterface &buffer) const override {
    ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;

    if ((0.0 != P_.get()) &&
        (::EmbeddedProto::Error::NO_ERRORS == return_value)) {
      return_value = P_.serialize_with_id(static_cast<uint32_t>(FieldNumber::P),
                                          buffer, false);
    }

    if ((0.0 != I_.get()) &&
        (::EmbeddedProto::Error::NO_ERRORS == return_value)) {
      return_value = I_.serialize_with_id(static_cast<uint32_t>(FieldNumber::I),
                                          buffer, false);
    }

    if ((0.0 != D_.get()) &&
        (::EmbeddedProto::Error::NO_ERRORS == return_value)) {
      return_value = D_.serialize_with_id(static_cast<uint32_t>(FieldNumber::D),
                                          buffer, false);
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
      case FieldNumber::P:
        return_value = P_.deserialize_check_type(buffer, wire_type);
        break;

      case FieldNumber::I:
        return_value = I_.deserialize_check_type(buffer, wire_type);
        break;

      case FieldNumber::D:
        return_value = D_.deserialize_check_type(buffer, wire_type);
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
    clear_P();
    clear_I();
    clear_D();
  }

  static char const *field_number_to_name(const FieldNumber fieldNumber) {
    char const *name = nullptr;
    switch (fieldNumber) {
    case FieldNumber::P:
      name = P_NAME;
      break;
    case FieldNumber::I:
      name = I_NAME;
      break;
    case FieldNumber::D:
      name = D_NAME;
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

    left_chars = P_.to_string(left_chars, indent_level + 2, P_NAME, true);
    left_chars = I_.to_string(left_chars, indent_level + 2, I_NAME, false);
    left_chars = D_.to_string(left_chars, indent_level + 2, D_NAME, false);

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
  EmbeddedProto::doublefixed P_ = 0.0;
  EmbeddedProto::doublefixed I_ = 0.0;
  EmbeddedProto::doublefixed D_ = 0.0;
};

class BoilerConfig final : public ::EmbeddedProto::MessageInterface {
public:
  BoilerConfig() = default;
  BoilerConfig(const BoilerConfig &rhs) {
    set_PID(rhs.get_PID());
    set_rref(rhs.get_rref());
    set_rnominal(rhs.get_rnominal());
  }

  BoilerConfig(const BoilerConfig &&rhs) noexcept {
    set_PID(rhs.get_PID());
    set_rref(rhs.get_rref());
    set_rnominal(rhs.get_rnominal());
  }

  ~BoilerConfig() override = default;

  enum class FieldNumber : uint32_t {
    NOT_SET = 0,
    PID = 1,
    RREF = 2,
    RNOMINAL = 3
  };

  BoilerConfig &operator=(const BoilerConfig &rhs) {
    set_PID(rhs.get_PID());
    set_rref(rhs.get_rref());
    set_rnominal(rhs.get_rnominal());
    return *this;
  }

  BoilerConfig &operator=(const BoilerConfig &&rhs) noexcept {
    set_PID(rhs.get_PID());
    set_rref(rhs.get_rref());
    set_rnominal(rhs.get_rnominal());
    return *this;
  }

  static constexpr char const *PID_NAME = "PID";
  inline void clear_PID() { PID_.clear(); }
  inline void set_PID(const PIDTunings &value) { PID_ = value; }
  inline void set_PID(const PIDTunings &&value) { PID_ = value; }
  inline PIDTunings &mutable_PID() { return PID_; }
  inline const PIDTunings &get_PID() const { return PID_; }
  inline const PIDTunings &PID() const { return PID_; }

  static constexpr char const *RREF_NAME = "rref";
  inline void clear_rref() { rref_.clear(); }
  inline void set_rref(const double &value) { rref_ = value; }
  inline void set_rref(const double &&value) { rref_ = value; }
  inline double &mutable_rref() { return rref_.get(); }
  inline const double &get_rref() const { return rref_.get(); }
  inline double rref() const { return rref_.get(); }

  static constexpr char const *RNOMINAL_NAME = "rnominal";
  inline void clear_rnominal() { rnominal_.clear(); }
  inline void set_rnominal(const double &value) { rnominal_ = value; }
  inline void set_rnominal(const double &&value) { rnominal_ = value; }
  inline double &mutable_rnominal() { return rnominal_.get(); }
  inline const double &get_rnominal() const { return rnominal_.get(); }
  inline double rnominal() const { return rnominal_.get(); }

  ::EmbeddedProto::Error
  serialize(::EmbeddedProto::WriteBufferInterface &buffer) const override {
    ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;

    if (::EmbeddedProto::Error::NO_ERRORS == return_value) {
      return_value = PID_.serialize_with_id(
          static_cast<uint32_t>(FieldNumber::PID), buffer, false);
    }

    if ((0.0 != rref_.get()) &&
        (::EmbeddedProto::Error::NO_ERRORS == return_value)) {
      return_value = rref_.serialize_with_id(
          static_cast<uint32_t>(FieldNumber::RREF), buffer, false);
    }

    if ((0.0 != rnominal_.get()) &&
        (::EmbeddedProto::Error::NO_ERRORS == return_value)) {
      return_value = rnominal_.serialize_with_id(
          static_cast<uint32_t>(FieldNumber::RNOMINAL), buffer, false);
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
      case FieldNumber::PID:
        return_value = PID_.deserialize_check_type(buffer, wire_type);
        break;

      case FieldNumber::RREF:
        return_value = rref_.deserialize_check_type(buffer, wire_type);
        break;

      case FieldNumber::RNOMINAL:
        return_value = rnominal_.deserialize_check_type(buffer, wire_type);
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
    clear_PID();
    clear_rref();
    clear_rnominal();
  }

  static char const *field_number_to_name(const FieldNumber fieldNumber) {
    char const *name = nullptr;
    switch (fieldNumber) {
    case FieldNumber::PID:
      name = PID_NAME;
      break;
    case FieldNumber::RREF:
      name = RREF_NAME;
      break;
    case FieldNumber::RNOMINAL:
      name = RNOMINAL_NAME;
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

    left_chars = PID_.to_string(left_chars, indent_level + 2, PID_NAME, true);
    left_chars =
        rref_.to_string(left_chars, indent_level + 2, RREF_NAME, false);
    left_chars =
        rnominal_.to_string(left_chars, indent_level + 2, RNOMINAL_NAME, false);

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
  PIDTunings PID_;
  EmbeddedProto::doublefixed rref_ = 0.0;
  EmbeddedProto::doublefixed rnominal_ = 0.0;
};

class Config final : public ::EmbeddedProto::MessageInterface {
public:
  Config() = default;
  Config(const Config &rhs) {
    set_boiler(rhs.get_boiler());
    set_setpoint(rhs.get_setpoint());
    set_steamSetPoint(rhs.get_steamSetPoint());
  }

  Config(const Config &&rhs) noexcept {
    set_boiler(rhs.get_boiler());
    set_setpoint(rhs.get_setpoint());
    set_steamSetPoint(rhs.get_steamSetPoint());
  }

  ~Config() override = default;

  enum class FieldNumber : uint32_t {
    NOT_SET = 0,
    BOILER = 1,
    SETPOINT = 2,
    STEAMSETPOINT = 3
  };

  Config &operator=(const Config &rhs) {
    set_boiler(rhs.get_boiler());
    set_setpoint(rhs.get_setpoint());
    set_steamSetPoint(rhs.get_steamSetPoint());
    return *this;
  }

  Config &operator=(const Config &&rhs) noexcept {
    set_boiler(rhs.get_boiler());
    set_setpoint(rhs.get_setpoint());
    set_steamSetPoint(rhs.get_steamSetPoint());
    return *this;
  }

  static constexpr char const *BOILER_NAME = "boiler";
  inline void clear_boiler() { boiler_.clear(); }
  inline void set_boiler(const BoilerConfig &value) { boiler_ = value; }
  inline void set_boiler(const BoilerConfig &&value) { boiler_ = value; }
  inline BoilerConfig &mutable_boiler() { return boiler_; }
  inline const BoilerConfig &get_boiler() const { return boiler_; }
  inline const BoilerConfig &boiler() const { return boiler_; }

  static constexpr char const *SETPOINT_NAME = "setpoint";
  inline void clear_setpoint() { setpoint_.clear(); }
  inline void set_setpoint(const double &value) { setpoint_ = value; }
  inline void set_setpoint(const double &&value) { setpoint_ = value; }
  inline double &mutable_setpoint() { return setpoint_.get(); }
  inline const double &get_setpoint() const { return setpoint_.get(); }
  inline double setpoint() const { return setpoint_.get(); }

  static constexpr char const *STEAMSETPOINT_NAME = "steamSetPoint";
  inline void clear_steamSetPoint() { steamSetPoint_.clear(); }
  inline void set_steamSetPoint(const double &value) { steamSetPoint_ = value; }
  inline void set_steamSetPoint(const double &&value) {
    steamSetPoint_ = value;
  }
  inline double &mutable_steamSetPoint() { return steamSetPoint_.get(); }
  inline const double &get_steamSetPoint() const {
    return steamSetPoint_.get();
  }
  inline double steamSetPoint() const { return steamSetPoint_.get(); }

  ::EmbeddedProto::Error
  serialize(::EmbeddedProto::WriteBufferInterface &buffer) const override {
    ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;

    if (::EmbeddedProto::Error::NO_ERRORS == return_value) {
      return_value = boiler_.serialize_with_id(
          static_cast<uint32_t>(FieldNumber::BOILER), buffer, false);
    }

    if ((0.0 != setpoint_.get()) &&
        (::EmbeddedProto::Error::NO_ERRORS == return_value)) {
      return_value = setpoint_.serialize_with_id(
          static_cast<uint32_t>(FieldNumber::SETPOINT), buffer, false);
    }

    if ((0.0 != steamSetPoint_.get()) &&
        (::EmbeddedProto::Error::NO_ERRORS == return_value)) {
      return_value = steamSetPoint_.serialize_with_id(
          static_cast<uint32_t>(FieldNumber::STEAMSETPOINT), buffer, false);
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
      case FieldNumber::BOILER:
        return_value = boiler_.deserialize_check_type(buffer, wire_type);
        break;

      case FieldNumber::SETPOINT:
        return_value = setpoint_.deserialize_check_type(buffer, wire_type);
        break;

      case FieldNumber::STEAMSETPOINT:
        return_value = steamSetPoint_.deserialize_check_type(buffer, wire_type);
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
    clear_boiler();
    clear_setpoint();
    clear_steamSetPoint();
  }

  static char const *field_number_to_name(const FieldNumber fieldNumber) {
    char const *name = nullptr;
    switch (fieldNumber) {
    case FieldNumber::BOILER:
      name = BOILER_NAME;
      break;
    case FieldNumber::SETPOINT:
      name = SETPOINT_NAME;
      break;
    case FieldNumber::STEAMSETPOINT:
      name = STEAMSETPOINT_NAME;
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
        boiler_.to_string(left_chars, indent_level + 2, BOILER_NAME, true);
    left_chars =
        setpoint_.to_string(left_chars, indent_level + 2, SETPOINT_NAME, false);
    left_chars = steamSetPoint_.to_string(left_chars, indent_level + 2,
                                          STEAMSETPOINT_NAME, false);

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
  BoilerConfig boiler_;
  EmbeddedProto::doublefixed setpoint_ = 0.0;
  EmbeddedProto::doublefixed steamSetPoint_ = 0.0;
};

#endif // CONFIG_H