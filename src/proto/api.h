/*
 *  Copyright (C) 2020-2023 Embedded AMS B.V. - All Rights Reserved
 *
 *  This file is part of Embedded Proto.
 *
 *  Embedded Proto is open source software: you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as published
 *  by the Free Software Foundation, version 3 of the license.
 *
 *  Embedded Proto  is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Embedded Proto. If not, see <https://www.gnu.org/licenses/>.
 *
 *  For commercial and closed source application please visit:
 *  <https://EmbeddedProto.com/license/>.
 *
 *  Embedded AMS B.V.
 *  Info:
 *    info at EmbeddedProto dot com
 *
 *  Postal address:
 *    Johan Huizingalaan 763a
 *    1066 VH, Amsterdam
 *    the Netherlands
 */

// This file is generated. Please do not edit!
#ifndef API_H
#define API_H

#include <cstdint>
#include <MessageInterface.h>
#include <WireFormatter.h>
#include <Fields.h>
#include <MessageSizeCalculator.h>
#include <ReadBufferSection.h>
#include <RepeatedFieldFixedSize.h>
#include <FieldStringBytes.h>
#include <Errors.h>
#include <Defines.h>
#include <limits>

// Include external proto definitions


class PowerOff final: public ::EmbeddedProto::MessageInterface
{
  public:
    PowerOff() = default;
    PowerOff(const PowerOff& rhs )
    {
    }

    PowerOff(const PowerOff&& rhs ) noexcept
    {
    }

    ~PowerOff() override = default;

    enum class FieldNumber : uint32_t
    {
      NOT_SET = 0,
    };

    PowerOff& operator=(const PowerOff& rhs)
    {
      return *this;
    }

    PowerOff& operator=(const PowerOff&& rhs) noexcept
    {
      return *this;
    }


    ::EmbeddedProto::Error serialize(::EmbeddedProto::WriteBufferInterface& buffer) const override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;

      return return_value;
    };

    ::EmbeddedProto::Error deserialize(::EmbeddedProto::ReadBufferInterface& buffer) override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;
      ::EmbeddedProto::WireFormatter::WireType wire_type = ::EmbeddedProto::WireFormatter::WireType::VARINT;
      uint32_t id_number = 0;
      FieldNumber id_tag = FieldNumber::NOT_SET;

      ::EmbeddedProto::Error tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
      while((::EmbeddedProto::Error::NO_ERRORS == return_value) && (::EmbeddedProto::Error::NO_ERRORS == tag_value))
      {
        id_tag = static_cast<FieldNumber>(id_number);
        switch(id_tag)
        {
          case FieldNumber::NOT_SET:
            return_value = ::EmbeddedProto::Error::INVALID_FIELD_ID;
            break;

          default:
            return_value = skip_unknown_field(buffer, wire_type);
            break;
        }

        if(::EmbeddedProto::Error::NO_ERRORS == return_value)
        {
          // Read the next tag.
          tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
        }
      }

      // When an error was detect while reading the tag but no other errors where found, set it in the return value.
      if((::EmbeddedProto::Error::NO_ERRORS == return_value)
         && (::EmbeddedProto::Error::NO_ERRORS != tag_value)
         && (::EmbeddedProto::Error::END_OF_BUFFER != tag_value)) // The end of the buffer is not an array in this case.
      {
        return_value = tag_value;
      }

      return return_value;
    };

    void clear() override
    {

    }

    static char const* field_number_to_name(const FieldNumber fieldNumber)
    {
      char const* name = nullptr;
      switch(fieldNumber)
      {
        default:
          name = "Invalid FieldNumber";
          break;
      }
      return name;
    }

#ifdef MSG_TO_STRING

    ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str) const
    {
      return this->to_string(str, 0, nullptr, true);
    }

    ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str, const uint32_t indent_level, char const* name, const bool first_field) const override
    {
      ::EmbeddedProto::string_view left_chars = str;
      int32_t n_chars_used = 0;

      if(!first_field)
      {
        // Add a comma behind the previous field.
        n_chars_used = snprintf(left_chars.data, left_chars.size, ",\n");
        if(0 < n_chars_used)
        {
          // Update the character pointer and characters left in the array.
          left_chars.data += n_chars_used;
          left_chars.size -= n_chars_used;
        }
      }

      if(nullptr != name)
      {
        if( 0 == indent_level)
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "\"%s\": {\n", name);
        }
        else
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s\"%s\": {\n", indent_level, " ", name);
        }
      }
      else
      {
        if( 0 == indent_level)
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "{\n");
        }
        else
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s{\n", indent_level, " ");
        }
      }
      
      if(0 < n_chars_used)
      {
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }

  
      if( 0 == indent_level) 
      {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "\n}");
      }
      else 
      {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "\n%*s}", indent_level, " ");
      }

      if(0 < n_chars_used)
      {
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }

      return left_chars;
    }

#endif // End of MSG_TO_STRING

  private:



};

class PowerOn final: public ::EmbeddedProto::MessageInterface
{
  public:
    PowerOn() = default;
    PowerOn(const PowerOn& rhs )
    {
    }

    PowerOn(const PowerOn&& rhs ) noexcept
    {
    }

    ~PowerOn() override = default;

    enum class FieldNumber : uint32_t
    {
      NOT_SET = 0,
    };

    PowerOn& operator=(const PowerOn& rhs)
    {
      return *this;
    }

    PowerOn& operator=(const PowerOn&& rhs) noexcept
    {
      return *this;
    }


    ::EmbeddedProto::Error serialize(::EmbeddedProto::WriteBufferInterface& buffer) const override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;

      return return_value;
    };

    ::EmbeddedProto::Error deserialize(::EmbeddedProto::ReadBufferInterface& buffer) override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;
      ::EmbeddedProto::WireFormatter::WireType wire_type = ::EmbeddedProto::WireFormatter::WireType::VARINT;
      uint32_t id_number = 0;
      FieldNumber id_tag = FieldNumber::NOT_SET;

      ::EmbeddedProto::Error tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
      while((::EmbeddedProto::Error::NO_ERRORS == return_value) && (::EmbeddedProto::Error::NO_ERRORS == tag_value))
      {
        id_tag = static_cast<FieldNumber>(id_number);
        switch(id_tag)
        {
          case FieldNumber::NOT_SET:
            return_value = ::EmbeddedProto::Error::INVALID_FIELD_ID;
            break;

          default:
            return_value = skip_unknown_field(buffer, wire_type);
            break;
        }

        if(::EmbeddedProto::Error::NO_ERRORS == return_value)
        {
          // Read the next tag.
          tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
        }
      }

      // When an error was detect while reading the tag but no other errors where found, set it in the return value.
      if((::EmbeddedProto::Error::NO_ERRORS == return_value)
         && (::EmbeddedProto::Error::NO_ERRORS != tag_value)
         && (::EmbeddedProto::Error::END_OF_BUFFER != tag_value)) // The end of the buffer is not an array in this case.
      {
        return_value = tag_value;
      }

      return return_value;
    };

    void clear() override
    {

    }

    static char const* field_number_to_name(const FieldNumber fieldNumber)
    {
      char const* name = nullptr;
      switch(fieldNumber)
      {
        default:
          name = "Invalid FieldNumber";
          break;
      }
      return name;
    }

#ifdef MSG_TO_STRING

    ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str) const
    {
      return this->to_string(str, 0, nullptr, true);
    }

    ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str, const uint32_t indent_level, char const* name, const bool first_field) const override
    {
      ::EmbeddedProto::string_view left_chars = str;
      int32_t n_chars_used = 0;

      if(!first_field)
      {
        // Add a comma behind the previous field.
        n_chars_used = snprintf(left_chars.data, left_chars.size, ",\n");
        if(0 < n_chars_used)
        {
          // Update the character pointer and characters left in the array.
          left_chars.data += n_chars_used;
          left_chars.size -= n_chars_used;
        }
      }

      if(nullptr != name)
      {
        if( 0 == indent_level)
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "\"%s\": {\n", name);
        }
        else
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s\"%s\": {\n", indent_level, " ", name);
        }
      }
      else
      {
        if( 0 == indent_level)
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "{\n");
        }
        else
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s{\n", indent_level, " ");
        }
      }
      
      if(0 < n_chars_used)
      {
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }

  
      if( 0 == indent_level) 
      {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "\n}");
      }
      else 
      {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "\n%*s}", indent_level, " ");
      }

      if(0 < n_chars_used)
      {
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }

      return left_chars;
    }

#endif // End of MSG_TO_STRING

  private:



};

class SetBoilerSetPoint final: public ::EmbeddedProto::MessageInterface
{
  public:
    SetBoilerSetPoint() = default;
    SetBoilerSetPoint(const SetBoilerSetPoint& rhs )
    {
    }

    SetBoilerSetPoint(const SetBoilerSetPoint&& rhs ) noexcept
    {
    }

    ~SetBoilerSetPoint() override = default;

    enum class FieldNumber : uint32_t
    {
      NOT_SET = 0,
    };

    SetBoilerSetPoint& operator=(const SetBoilerSetPoint& rhs)
    {
      return *this;
    }

    SetBoilerSetPoint& operator=(const SetBoilerSetPoint&& rhs) noexcept
    {
      return *this;
    }


    ::EmbeddedProto::Error serialize(::EmbeddedProto::WriteBufferInterface& buffer) const override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;

      return return_value;
    };

    ::EmbeddedProto::Error deserialize(::EmbeddedProto::ReadBufferInterface& buffer) override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;
      ::EmbeddedProto::WireFormatter::WireType wire_type = ::EmbeddedProto::WireFormatter::WireType::VARINT;
      uint32_t id_number = 0;
      FieldNumber id_tag = FieldNumber::NOT_SET;

      ::EmbeddedProto::Error tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
      while((::EmbeddedProto::Error::NO_ERRORS == return_value) && (::EmbeddedProto::Error::NO_ERRORS == tag_value))
      {
        id_tag = static_cast<FieldNumber>(id_number);
        switch(id_tag)
        {
          case FieldNumber::NOT_SET:
            return_value = ::EmbeddedProto::Error::INVALID_FIELD_ID;
            break;

          default:
            return_value = skip_unknown_field(buffer, wire_type);
            break;
        }

        if(::EmbeddedProto::Error::NO_ERRORS == return_value)
        {
          // Read the next tag.
          tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
        }
      }

      // When an error was detect while reading the tag but no other errors where found, set it in the return value.
      if((::EmbeddedProto::Error::NO_ERRORS == return_value)
         && (::EmbeddedProto::Error::NO_ERRORS != tag_value)
         && (::EmbeddedProto::Error::END_OF_BUFFER != tag_value)) // The end of the buffer is not an array in this case.
      {
        return_value = tag_value;
      }

      return return_value;
    };

    void clear() override
    {

    }

    static char const* field_number_to_name(const FieldNumber fieldNumber)
    {
      char const* name = nullptr;
      switch(fieldNumber)
      {
        default:
          name = "Invalid FieldNumber";
          break;
      }
      return name;
    }

#ifdef MSG_TO_STRING

    ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str) const
    {
      return this->to_string(str, 0, nullptr, true);
    }

    ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str, const uint32_t indent_level, char const* name, const bool first_field) const override
    {
      ::EmbeddedProto::string_view left_chars = str;
      int32_t n_chars_used = 0;

      if(!first_field)
      {
        // Add a comma behind the previous field.
        n_chars_used = snprintf(left_chars.data, left_chars.size, ",\n");
        if(0 < n_chars_used)
        {
          // Update the character pointer and characters left in the array.
          left_chars.data += n_chars_used;
          left_chars.size -= n_chars_used;
        }
      }

      if(nullptr != name)
      {
        if( 0 == indent_level)
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "\"%s\": {\n", name);
        }
        else
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s\"%s\": {\n", indent_level, " ", name);
        }
      }
      else
      {
        if( 0 == indent_level)
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "{\n");
        }
        else
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s{\n", indent_level, " ");
        }
      }
      
      if(0 < n_chars_used)
      {
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }

  
      if( 0 == indent_level) 
      {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "\n}");
      }
      else 
      {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "\n%*s}", indent_level, " ");
      }

      if(0 < n_chars_used)
      {
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }

      return left_chars;
    }

#endif // End of MSG_TO_STRING

  private:



};

class SetSteamSetPoint final: public ::EmbeddedProto::MessageInterface
{
  public:
    SetSteamSetPoint() = default;
    SetSteamSetPoint(const SetSteamSetPoint& rhs )
    {
    }

    SetSteamSetPoint(const SetSteamSetPoint&& rhs ) noexcept
    {
    }

    ~SetSteamSetPoint() override = default;

    enum class FieldNumber : uint32_t
    {
      NOT_SET = 0,
    };

    SetSteamSetPoint& operator=(const SetSteamSetPoint& rhs)
    {
      return *this;
    }

    SetSteamSetPoint& operator=(const SetSteamSetPoint&& rhs) noexcept
    {
      return *this;
    }


    ::EmbeddedProto::Error serialize(::EmbeddedProto::WriteBufferInterface& buffer) const override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;

      return return_value;
    };

    ::EmbeddedProto::Error deserialize(::EmbeddedProto::ReadBufferInterface& buffer) override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;
      ::EmbeddedProto::WireFormatter::WireType wire_type = ::EmbeddedProto::WireFormatter::WireType::VARINT;
      uint32_t id_number = 0;
      FieldNumber id_tag = FieldNumber::NOT_SET;

      ::EmbeddedProto::Error tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
      while((::EmbeddedProto::Error::NO_ERRORS == return_value) && (::EmbeddedProto::Error::NO_ERRORS == tag_value))
      {
        id_tag = static_cast<FieldNumber>(id_number);
        switch(id_tag)
        {
          case FieldNumber::NOT_SET:
            return_value = ::EmbeddedProto::Error::INVALID_FIELD_ID;
            break;

          default:
            return_value = skip_unknown_field(buffer, wire_type);
            break;
        }

        if(::EmbeddedProto::Error::NO_ERRORS == return_value)
        {
          // Read the next tag.
          tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
        }
      }

      // When an error was detect while reading the tag but no other errors where found, set it in the return value.
      if((::EmbeddedProto::Error::NO_ERRORS == return_value)
         && (::EmbeddedProto::Error::NO_ERRORS != tag_value)
         && (::EmbeddedProto::Error::END_OF_BUFFER != tag_value)) // The end of the buffer is not an array in this case.
      {
        return_value = tag_value;
      }

      return return_value;
    };

    void clear() override
    {

    }

    static char const* field_number_to_name(const FieldNumber fieldNumber)
    {
      char const* name = nullptr;
      switch(fieldNumber)
      {
        default:
          name = "Invalid FieldNumber";
          break;
      }
      return name;
    }

#ifdef MSG_TO_STRING

    ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str) const
    {
      return this->to_string(str, 0, nullptr, true);
    }

    ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str, const uint32_t indent_level, char const* name, const bool first_field) const override
    {
      ::EmbeddedProto::string_view left_chars = str;
      int32_t n_chars_used = 0;

      if(!first_field)
      {
        // Add a comma behind the previous field.
        n_chars_used = snprintf(left_chars.data, left_chars.size, ",\n");
        if(0 < n_chars_used)
        {
          // Update the character pointer and characters left in the array.
          left_chars.data += n_chars_used;
          left_chars.size -= n_chars_used;
        }
      }

      if(nullptr != name)
      {
        if( 0 == indent_level)
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "\"%s\": {\n", name);
        }
        else
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s\"%s\": {\n", indent_level, " ", name);
        }
      }
      else
      {
        if( 0 == indent_level)
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "{\n");
        }
        else
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s{\n", indent_level, " ");
        }
      }
      
      if(0 < n_chars_used)
      {
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }

  
      if( 0 == indent_level) 
      {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "\n}");
      }
      else 
      {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "\n%*s}", indent_level, " ");
      }

      if(0 < n_chars_used)
      {
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }

      return left_chars;
    }

#endif // End of MSG_TO_STRING

  private:



};

class StartBrew final: public ::EmbeddedProto::MessageInterface
{
  public:
    StartBrew() = default;
    StartBrew(const StartBrew& rhs )
    {
    }

    StartBrew(const StartBrew&& rhs ) noexcept
    {
    }

    ~StartBrew() override = default;

    enum class FieldNumber : uint32_t
    {
      NOT_SET = 0,
    };

    StartBrew& operator=(const StartBrew& rhs)
    {
      return *this;
    }

    StartBrew& operator=(const StartBrew&& rhs) noexcept
    {
      return *this;
    }


    ::EmbeddedProto::Error serialize(::EmbeddedProto::WriteBufferInterface& buffer) const override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;

      return return_value;
    };

    ::EmbeddedProto::Error deserialize(::EmbeddedProto::ReadBufferInterface& buffer) override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;
      ::EmbeddedProto::WireFormatter::WireType wire_type = ::EmbeddedProto::WireFormatter::WireType::VARINT;
      uint32_t id_number = 0;
      FieldNumber id_tag = FieldNumber::NOT_SET;

      ::EmbeddedProto::Error tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
      while((::EmbeddedProto::Error::NO_ERRORS == return_value) && (::EmbeddedProto::Error::NO_ERRORS == tag_value))
      {
        id_tag = static_cast<FieldNumber>(id_number);
        switch(id_tag)
        {
          case FieldNumber::NOT_SET:
            return_value = ::EmbeddedProto::Error::INVALID_FIELD_ID;
            break;

          default:
            return_value = skip_unknown_field(buffer, wire_type);
            break;
        }

        if(::EmbeddedProto::Error::NO_ERRORS == return_value)
        {
          // Read the next tag.
          tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
        }
      }

      // When an error was detect while reading the tag but no other errors where found, set it in the return value.
      if((::EmbeddedProto::Error::NO_ERRORS == return_value)
         && (::EmbeddedProto::Error::NO_ERRORS != tag_value)
         && (::EmbeddedProto::Error::END_OF_BUFFER != tag_value)) // The end of the buffer is not an array in this case.
      {
        return_value = tag_value;
      }

      return return_value;
    };

    void clear() override
    {

    }

    static char const* field_number_to_name(const FieldNumber fieldNumber)
    {
      char const* name = nullptr;
      switch(fieldNumber)
      {
        default:
          name = "Invalid FieldNumber";
          break;
      }
      return name;
    }

#ifdef MSG_TO_STRING

    ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str) const
    {
      return this->to_string(str, 0, nullptr, true);
    }

    ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str, const uint32_t indent_level, char const* name, const bool first_field) const override
    {
      ::EmbeddedProto::string_view left_chars = str;
      int32_t n_chars_used = 0;

      if(!first_field)
      {
        // Add a comma behind the previous field.
        n_chars_used = snprintf(left_chars.data, left_chars.size, ",\n");
        if(0 < n_chars_used)
        {
          // Update the character pointer and characters left in the array.
          left_chars.data += n_chars_used;
          left_chars.size -= n_chars_used;
        }
      }

      if(nullptr != name)
      {
        if( 0 == indent_level)
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "\"%s\": {\n", name);
        }
        else
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s\"%s\": {\n", indent_level, " ", name);
        }
      }
      else
      {
        if( 0 == indent_level)
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "{\n");
        }
        else
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s{\n", indent_level, " ");
        }
      }
      
      if(0 < n_chars_used)
      {
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }

  
      if( 0 == indent_level) 
      {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "\n}");
      }
      else 
      {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "\n%*s}", indent_level, " ");
      }

      if(0 < n_chars_used)
      {
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }

      return left_chars;
    }

#endif // End of MSG_TO_STRING

  private:



};

class StartSteam final: public ::EmbeddedProto::MessageInterface
{
  public:
    StartSteam() = default;
    StartSteam(const StartSteam& rhs )
    {
    }

    StartSteam(const StartSteam&& rhs ) noexcept
    {
    }

    ~StartSteam() override = default;

    enum class FieldNumber : uint32_t
    {
      NOT_SET = 0,
    };

    StartSteam& operator=(const StartSteam& rhs)
    {
      return *this;
    }

    StartSteam& operator=(const StartSteam&& rhs) noexcept
    {
      return *this;
    }


    ::EmbeddedProto::Error serialize(::EmbeddedProto::WriteBufferInterface& buffer) const override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;

      return return_value;
    };

    ::EmbeddedProto::Error deserialize(::EmbeddedProto::ReadBufferInterface& buffer) override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;
      ::EmbeddedProto::WireFormatter::WireType wire_type = ::EmbeddedProto::WireFormatter::WireType::VARINT;
      uint32_t id_number = 0;
      FieldNumber id_tag = FieldNumber::NOT_SET;

      ::EmbeddedProto::Error tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
      while((::EmbeddedProto::Error::NO_ERRORS == return_value) && (::EmbeddedProto::Error::NO_ERRORS == tag_value))
      {
        id_tag = static_cast<FieldNumber>(id_number);
        switch(id_tag)
        {
          case FieldNumber::NOT_SET:
            return_value = ::EmbeddedProto::Error::INVALID_FIELD_ID;
            break;

          default:
            return_value = skip_unknown_field(buffer, wire_type);
            break;
        }

        if(::EmbeddedProto::Error::NO_ERRORS == return_value)
        {
          // Read the next tag.
          tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
        }
      }

      // When an error was detect while reading the tag but no other errors where found, set it in the return value.
      if((::EmbeddedProto::Error::NO_ERRORS == return_value)
         && (::EmbeddedProto::Error::NO_ERRORS != tag_value)
         && (::EmbeddedProto::Error::END_OF_BUFFER != tag_value)) // The end of the buffer is not an array in this case.
      {
        return_value = tag_value;
      }

      return return_value;
    };

    void clear() override
    {

    }

    static char const* field_number_to_name(const FieldNumber fieldNumber)
    {
      char const* name = nullptr;
      switch(fieldNumber)
      {
        default:
          name = "Invalid FieldNumber";
          break;
      }
      return name;
    }

#ifdef MSG_TO_STRING

    ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str) const
    {
      return this->to_string(str, 0, nullptr, true);
    }

    ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str, const uint32_t indent_level, char const* name, const bool first_field) const override
    {
      ::EmbeddedProto::string_view left_chars = str;
      int32_t n_chars_used = 0;

      if(!first_field)
      {
        // Add a comma behind the previous field.
        n_chars_used = snprintf(left_chars.data, left_chars.size, ",\n");
        if(0 < n_chars_used)
        {
          // Update the character pointer and characters left in the array.
          left_chars.data += n_chars_used;
          left_chars.size -= n_chars_used;
        }
      }

      if(nullptr != name)
      {
        if( 0 == indent_level)
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "\"%s\": {\n", name);
        }
        else
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s\"%s\": {\n", indent_level, " ", name);
        }
      }
      else
      {
        if( 0 == indent_level)
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "{\n");
        }
        else
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s{\n", indent_level, " ");
        }
      }
      
      if(0 < n_chars_used)
      {
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }

  
      if( 0 == indent_level) 
      {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "\n}");
      }
      else 
      {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "\n%*s}", indent_level, " ");
      }

      if(0 < n_chars_used)
      {
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }

      return left_chars;
    }

#endif // End of MSG_TO_STRING

  private:



};

class StopBrew final: public ::EmbeddedProto::MessageInterface
{
  public:
    StopBrew() = default;
    StopBrew(const StopBrew& rhs )
    {
    }

    StopBrew(const StopBrew&& rhs ) noexcept
    {
    }

    ~StopBrew() override = default;

    enum class FieldNumber : uint32_t
    {
      NOT_SET = 0,
    };

    StopBrew& operator=(const StopBrew& rhs)
    {
      return *this;
    }

    StopBrew& operator=(const StopBrew&& rhs) noexcept
    {
      return *this;
    }


    ::EmbeddedProto::Error serialize(::EmbeddedProto::WriteBufferInterface& buffer) const override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;

      return return_value;
    };

    ::EmbeddedProto::Error deserialize(::EmbeddedProto::ReadBufferInterface& buffer) override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;
      ::EmbeddedProto::WireFormatter::WireType wire_type = ::EmbeddedProto::WireFormatter::WireType::VARINT;
      uint32_t id_number = 0;
      FieldNumber id_tag = FieldNumber::NOT_SET;

      ::EmbeddedProto::Error tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
      while((::EmbeddedProto::Error::NO_ERRORS == return_value) && (::EmbeddedProto::Error::NO_ERRORS == tag_value))
      {
        id_tag = static_cast<FieldNumber>(id_number);
        switch(id_tag)
        {
          case FieldNumber::NOT_SET:
            return_value = ::EmbeddedProto::Error::INVALID_FIELD_ID;
            break;

          default:
            return_value = skip_unknown_field(buffer, wire_type);
            break;
        }

        if(::EmbeddedProto::Error::NO_ERRORS == return_value)
        {
          // Read the next tag.
          tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
        }
      }

      // When an error was detect while reading the tag but no other errors where found, set it in the return value.
      if((::EmbeddedProto::Error::NO_ERRORS == return_value)
         && (::EmbeddedProto::Error::NO_ERRORS != tag_value)
         && (::EmbeddedProto::Error::END_OF_BUFFER != tag_value)) // The end of the buffer is not an array in this case.
      {
        return_value = tag_value;
      }

      return return_value;
    };

    void clear() override
    {

    }

    static char const* field_number_to_name(const FieldNumber fieldNumber)
    {
      char const* name = nullptr;
      switch(fieldNumber)
      {
        default:
          name = "Invalid FieldNumber";
          break;
      }
      return name;
    }

#ifdef MSG_TO_STRING

    ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str) const
    {
      return this->to_string(str, 0, nullptr, true);
    }

    ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str, const uint32_t indent_level, char const* name, const bool first_field) const override
    {
      ::EmbeddedProto::string_view left_chars = str;
      int32_t n_chars_used = 0;

      if(!first_field)
      {
        // Add a comma behind the previous field.
        n_chars_used = snprintf(left_chars.data, left_chars.size, ",\n");
        if(0 < n_chars_used)
        {
          // Update the character pointer and characters left in the array.
          left_chars.data += n_chars_used;
          left_chars.size -= n_chars_used;
        }
      }

      if(nullptr != name)
      {
        if( 0 == indent_level)
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "\"%s\": {\n", name);
        }
        else
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s\"%s\": {\n", indent_level, " ", name);
        }
      }
      else
      {
        if( 0 == indent_level)
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "{\n");
        }
        else
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s{\n", indent_level, " ");
        }
      }
      
      if(0 < n_chars_used)
      {
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }

  
      if( 0 == indent_level) 
      {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "\n}");
      }
      else 
      {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "\n%*s}", indent_level, " ");
      }

      if(0 < n_chars_used)
      {
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }

      return left_chars;
    }

#endif // End of MSG_TO_STRING

  private:



};

class StopSteam final: public ::EmbeddedProto::MessageInterface
{
  public:
    StopSteam() = default;
    StopSteam(const StopSteam& rhs )
    {
    }

    StopSteam(const StopSteam&& rhs ) noexcept
    {
    }

    ~StopSteam() override = default;

    enum class FieldNumber : uint32_t
    {
      NOT_SET = 0,
    };

    StopSteam& operator=(const StopSteam& rhs)
    {
      return *this;
    }

    StopSteam& operator=(const StopSteam&& rhs) noexcept
    {
      return *this;
    }


    ::EmbeddedProto::Error serialize(::EmbeddedProto::WriteBufferInterface& buffer) const override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;

      return return_value;
    };

    ::EmbeddedProto::Error deserialize(::EmbeddedProto::ReadBufferInterface& buffer) override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;
      ::EmbeddedProto::WireFormatter::WireType wire_type = ::EmbeddedProto::WireFormatter::WireType::VARINT;
      uint32_t id_number = 0;
      FieldNumber id_tag = FieldNumber::NOT_SET;

      ::EmbeddedProto::Error tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
      while((::EmbeddedProto::Error::NO_ERRORS == return_value) && (::EmbeddedProto::Error::NO_ERRORS == tag_value))
      {
        id_tag = static_cast<FieldNumber>(id_number);
        switch(id_tag)
        {
          case FieldNumber::NOT_SET:
            return_value = ::EmbeddedProto::Error::INVALID_FIELD_ID;
            break;

          default:
            return_value = skip_unknown_field(buffer, wire_type);
            break;
        }

        if(::EmbeddedProto::Error::NO_ERRORS == return_value)
        {
          // Read the next tag.
          tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
        }
      }

      // When an error was detect while reading the tag but no other errors where found, set it in the return value.
      if((::EmbeddedProto::Error::NO_ERRORS == return_value)
         && (::EmbeddedProto::Error::NO_ERRORS != tag_value)
         && (::EmbeddedProto::Error::END_OF_BUFFER != tag_value)) // The end of the buffer is not an array in this case.
      {
        return_value = tag_value;
      }

      return return_value;
    };

    void clear() override
    {

    }

    static char const* field_number_to_name(const FieldNumber fieldNumber)
    {
      char const* name = nullptr;
      switch(fieldNumber)
      {
        default:
          name = "Invalid FieldNumber";
          break;
      }
      return name;
    }

#ifdef MSG_TO_STRING

    ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str) const
    {
      return this->to_string(str, 0, nullptr, true);
    }

    ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str, const uint32_t indent_level, char const* name, const bool first_field) const override
    {
      ::EmbeddedProto::string_view left_chars = str;
      int32_t n_chars_used = 0;

      if(!first_field)
      {
        // Add a comma behind the previous field.
        n_chars_used = snprintf(left_chars.data, left_chars.size, ",\n");
        if(0 < n_chars_used)
        {
          // Update the character pointer and characters left in the array.
          left_chars.data += n_chars_used;
          left_chars.size -= n_chars_used;
        }
      }

      if(nullptr != name)
      {
        if( 0 == indent_level)
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "\"%s\": {\n", name);
        }
        else
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s\"%s\": {\n", indent_level, " ", name);
        }
      }
      else
      {
        if( 0 == indent_level)
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "{\n");
        }
        else
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s{\n", indent_level, " ");
        }
      }
      
      if(0 < n_chars_used)
      {
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }

  
      if( 0 == indent_level) 
      {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "\n}");
      }
      else 
      {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "\n%*s}", indent_level, " ");
      }

      if(0 < n_chars_used)
      {
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }

      return left_chars;
    }

#endif // End of MSG_TO_STRING

  private:



};

template<
    uint32_t Command_request_id_LENGTH
>
class Command final: public ::EmbeddedProto::MessageInterface
{
  public:
    Command() = default;
    Command(const Command& rhs )
    {
      set_request_id(rhs.get_request_id());
      if(rhs.get_which_command_oneof() != which_command_oneof_)
      {
        // First delete the old object in the oneof.
        clear_command_oneof();
      }

      switch(rhs.get_which_command_oneof())
      {
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

        case FieldNumber::START_STEAM:
          set_start_steam(rhs.get_start_steam());
          break;

        case FieldNumber::STOP_STEAM:
          set_stop_steam(rhs.get_stop_steam());
          break;

        case FieldNumber::SET_STEAM_SET_POINT:
          set_set_steam_set_point(rhs.get_set_steam_set_point());
          break;

        case FieldNumber::SET_BOILER_SET_POINT:
          set_set_boiler_set_point(rhs.get_set_boiler_set_point());
          break;

        default:
          break;
      }

    }

    Command(const Command&& rhs ) noexcept
    {
      set_request_id(rhs.get_request_id());
      if(rhs.get_which_command_oneof() != which_command_oneof_)
      {
        // First delete the old object in the oneof.
        clear_command_oneof();
      }

      switch(rhs.get_which_command_oneof())
      {
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

        case FieldNumber::START_STEAM:
          set_start_steam(rhs.get_start_steam());
          break;

        case FieldNumber::STOP_STEAM:
          set_stop_steam(rhs.get_stop_steam());
          break;

        case FieldNumber::SET_STEAM_SET_POINT:
          set_set_steam_set_point(rhs.get_set_steam_set_point());
          break;

        case FieldNumber::SET_BOILER_SET_POINT:
          set_set_boiler_set_point(rhs.get_set_boiler_set_point());
          break;

        default:
          break;
      }

    }

    ~Command() override = default;

    enum class FieldNumber : uint32_t
    {
      NOT_SET = 0,
      REQUEST_ID = 1,
      POWER_ON = 2,
      POWER_OFF = 3,
      START_BREW = 4,
      STOP_BREW = 5,
      START_STEAM = 6,
      STOP_STEAM = 7,
      SET_STEAM_SET_POINT = 8,
      SET_BOILER_SET_POINT = 9
    };

    Command& operator=(const Command& rhs)
    {
      set_request_id(rhs.get_request_id());
      if(rhs.get_which_command_oneof() != which_command_oneof_)
      {
        // First delete the old object in the oneof.
        clear_command_oneof();
      }

      switch(rhs.get_which_command_oneof())
      {
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

        case FieldNumber::START_STEAM:
          set_start_steam(rhs.get_start_steam());
          break;

        case FieldNumber::STOP_STEAM:
          set_stop_steam(rhs.get_stop_steam());
          break;

        case FieldNumber::SET_STEAM_SET_POINT:
          set_set_steam_set_point(rhs.get_set_steam_set_point());
          break;

        case FieldNumber::SET_BOILER_SET_POINT:
          set_set_boiler_set_point(rhs.get_set_boiler_set_point());
          break;

        default:
          break;
      }

      return *this;
    }

    Command& operator=(const Command&& rhs) noexcept
    {
      set_request_id(rhs.get_request_id());
      if(rhs.get_which_command_oneof() != which_command_oneof_)
      {
        // First delete the old object in the oneof.
        clear_command_oneof();
      }

      switch(rhs.get_which_command_oneof())
      {
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

        case FieldNumber::START_STEAM:
          set_start_steam(rhs.get_start_steam());
          break;

        case FieldNumber::STOP_STEAM:
          set_stop_steam(rhs.get_stop_steam());
          break;

        case FieldNumber::SET_STEAM_SET_POINT:
          set_set_steam_set_point(rhs.get_set_steam_set_point());
          break;

        case FieldNumber::SET_BOILER_SET_POINT:
          set_set_boiler_set_point(rhs.get_set_boiler_set_point());
          break;

        default:
          break;
      }

      return *this;
    }

    static constexpr char const* REQUEST_ID_NAME = "request_id";
    inline void clear_request_id() { request_id_.clear(); }
    inline ::EmbeddedProto::FieldString<Command_request_id_LENGTH>& mutable_request_id() { return request_id_; }
    inline void set_request_id(const ::EmbeddedProto::FieldString<Command_request_id_LENGTH>& rhs) { request_id_.set(rhs); }
    inline const ::EmbeddedProto::FieldString<Command_request_id_LENGTH>& get_request_id() const { return request_id_; }
    inline const char* request_id() const { return request_id_.get_const(); }

    FieldNumber get_which_command_oneof() const { return which_command_oneof_; }

    static constexpr char const* POWER_ON_NAME = "power_on";
    inline bool has_power_on() const
    {
      return FieldNumber::POWER_ON == which_command_oneof_;
    }
    inline void clear_power_on()
    {
      if(FieldNumber::POWER_ON == which_command_oneof_)
      {
        which_command_oneof_ = FieldNumber::NOT_SET;
        command_oneof_.power_on_.~PowerOn();
      }
    }
    inline void set_power_on(const PowerOn& value)
    {
      if(FieldNumber::POWER_ON != which_command_oneof_)
      {
        init_command_oneof(FieldNumber::POWER_ON);
      }
      command_oneof_.power_on_ = value;
    }
    inline void set_power_on(const PowerOn&& value)
    {
      if(FieldNumber::POWER_ON != which_command_oneof_)
      {
        init_command_oneof(FieldNumber::POWER_ON);
      }
      command_oneof_.power_on_ = value;
    }
    inline PowerOn& mutable_power_on()
    {
      if(FieldNumber::POWER_ON != which_command_oneof_)
      {
        init_command_oneof(FieldNumber::POWER_ON);
      }
      return command_oneof_.power_on_;
    }
    inline const PowerOn& get_power_on() const { return command_oneof_.power_on_; }
    inline const PowerOn& power_on() const { return command_oneof_.power_on_; }

    static constexpr char const* POWER_OFF_NAME = "power_off";
    inline bool has_power_off() const
    {
      return FieldNumber::POWER_OFF == which_command_oneof_;
    }
    inline void clear_power_off()
    {
      if(FieldNumber::POWER_OFF == which_command_oneof_)
      {
        which_command_oneof_ = FieldNumber::NOT_SET;
        command_oneof_.power_off_.~PowerOff();
      }
    }
    inline void set_power_off(const PowerOff& value)
    {
      if(FieldNumber::POWER_OFF != which_command_oneof_)
      {
        init_command_oneof(FieldNumber::POWER_OFF);
      }
      command_oneof_.power_off_ = value;
    }
    inline void set_power_off(const PowerOff&& value)
    {
      if(FieldNumber::POWER_OFF != which_command_oneof_)
      {
        init_command_oneof(FieldNumber::POWER_OFF);
      }
      command_oneof_.power_off_ = value;
    }
    inline PowerOff& mutable_power_off()
    {
      if(FieldNumber::POWER_OFF != which_command_oneof_)
      {
        init_command_oneof(FieldNumber::POWER_OFF);
      }
      return command_oneof_.power_off_;
    }
    inline const PowerOff& get_power_off() const { return command_oneof_.power_off_; }
    inline const PowerOff& power_off() const { return command_oneof_.power_off_; }

    static constexpr char const* START_BREW_NAME = "start_brew";
    inline bool has_start_brew() const
    {
      return FieldNumber::START_BREW == which_command_oneof_;
    }
    inline void clear_start_brew()
    {
      if(FieldNumber::START_BREW == which_command_oneof_)
      {
        which_command_oneof_ = FieldNumber::NOT_SET;
        command_oneof_.start_brew_.~StartBrew();
      }
    }
    inline void set_start_brew(const StartBrew& value)
    {
      if(FieldNumber::START_BREW != which_command_oneof_)
      {
        init_command_oneof(FieldNumber::START_BREW);
      }
      command_oneof_.start_brew_ = value;
    }
    inline void set_start_brew(const StartBrew&& value)
    {
      if(FieldNumber::START_BREW != which_command_oneof_)
      {
        init_command_oneof(FieldNumber::START_BREW);
      }
      command_oneof_.start_brew_ = value;
    }
    inline StartBrew& mutable_start_brew()
    {
      if(FieldNumber::START_BREW != which_command_oneof_)
      {
        init_command_oneof(FieldNumber::START_BREW);
      }
      return command_oneof_.start_brew_;
    }
    inline const StartBrew& get_start_brew() const { return command_oneof_.start_brew_; }
    inline const StartBrew& start_brew() const { return command_oneof_.start_brew_; }

    static constexpr char const* STOP_BREW_NAME = "stop_brew";
    inline bool has_stop_brew() const
    {
      return FieldNumber::STOP_BREW == which_command_oneof_;
    }
    inline void clear_stop_brew()
    {
      if(FieldNumber::STOP_BREW == which_command_oneof_)
      {
        which_command_oneof_ = FieldNumber::NOT_SET;
        command_oneof_.stop_brew_.~StopBrew();
      }
    }
    inline void set_stop_brew(const StopBrew& value)
    {
      if(FieldNumber::STOP_BREW != which_command_oneof_)
      {
        init_command_oneof(FieldNumber::STOP_BREW);
      }
      command_oneof_.stop_brew_ = value;
    }
    inline void set_stop_brew(const StopBrew&& value)
    {
      if(FieldNumber::STOP_BREW != which_command_oneof_)
      {
        init_command_oneof(FieldNumber::STOP_BREW);
      }
      command_oneof_.stop_brew_ = value;
    }
    inline StopBrew& mutable_stop_brew()
    {
      if(FieldNumber::STOP_BREW != which_command_oneof_)
      {
        init_command_oneof(FieldNumber::STOP_BREW);
      }
      return command_oneof_.stop_brew_;
    }
    inline const StopBrew& get_stop_brew() const { return command_oneof_.stop_brew_; }
    inline const StopBrew& stop_brew() const { return command_oneof_.stop_brew_; }

    static constexpr char const* START_STEAM_NAME = "start_steam";
    inline bool has_start_steam() const
    {
      return FieldNumber::START_STEAM == which_command_oneof_;
    }
    inline void clear_start_steam()
    {
      if(FieldNumber::START_STEAM == which_command_oneof_)
      {
        which_command_oneof_ = FieldNumber::NOT_SET;
        command_oneof_.start_steam_.~StartSteam();
      }
    }
    inline void set_start_steam(const StartSteam& value)
    {
      if(FieldNumber::START_STEAM != which_command_oneof_)
      {
        init_command_oneof(FieldNumber::START_STEAM);
      }
      command_oneof_.start_steam_ = value;
    }
    inline void set_start_steam(const StartSteam&& value)
    {
      if(FieldNumber::START_STEAM != which_command_oneof_)
      {
        init_command_oneof(FieldNumber::START_STEAM);
      }
      command_oneof_.start_steam_ = value;
    }
    inline StartSteam& mutable_start_steam()
    {
      if(FieldNumber::START_STEAM != which_command_oneof_)
      {
        init_command_oneof(FieldNumber::START_STEAM);
      }
      return command_oneof_.start_steam_;
    }
    inline const StartSteam& get_start_steam() const { return command_oneof_.start_steam_; }
    inline const StartSteam& start_steam() const { return command_oneof_.start_steam_; }

    static constexpr char const* STOP_STEAM_NAME = "stop_steam";
    inline bool has_stop_steam() const
    {
      return FieldNumber::STOP_STEAM == which_command_oneof_;
    }
    inline void clear_stop_steam()
    {
      if(FieldNumber::STOP_STEAM == which_command_oneof_)
      {
        which_command_oneof_ = FieldNumber::NOT_SET;
        command_oneof_.stop_steam_.~StopSteam();
      }
    }
    inline void set_stop_steam(const StopSteam& value)
    {
      if(FieldNumber::STOP_STEAM != which_command_oneof_)
      {
        init_command_oneof(FieldNumber::STOP_STEAM);
      }
      command_oneof_.stop_steam_ = value;
    }
    inline void set_stop_steam(const StopSteam&& value)
    {
      if(FieldNumber::STOP_STEAM != which_command_oneof_)
      {
        init_command_oneof(FieldNumber::STOP_STEAM);
      }
      command_oneof_.stop_steam_ = value;
    }
    inline StopSteam& mutable_stop_steam()
    {
      if(FieldNumber::STOP_STEAM != which_command_oneof_)
      {
        init_command_oneof(FieldNumber::STOP_STEAM);
      }
      return command_oneof_.stop_steam_;
    }
    inline const StopSteam& get_stop_steam() const { return command_oneof_.stop_steam_; }
    inline const StopSteam& stop_steam() const { return command_oneof_.stop_steam_; }

    static constexpr char const* SET_STEAM_SET_POINT_NAME = "set_steam_set_point";
    inline bool has_set_steam_set_point() const
    {
      return FieldNumber::SET_STEAM_SET_POINT == which_command_oneof_;
    }
    inline void clear_set_steam_set_point()
    {
      if(FieldNumber::SET_STEAM_SET_POINT == which_command_oneof_)
      {
        which_command_oneof_ = FieldNumber::NOT_SET;
        command_oneof_.set_steam_set_point_.~SetSteamSetPoint();
      }
    }
    inline void set_set_steam_set_point(const SetSteamSetPoint& value)
    {
      if(FieldNumber::SET_STEAM_SET_POINT != which_command_oneof_)
      {
        init_command_oneof(FieldNumber::SET_STEAM_SET_POINT);
      }
      command_oneof_.set_steam_set_point_ = value;
    }
    inline void set_set_steam_set_point(const SetSteamSetPoint&& value)
    {
      if(FieldNumber::SET_STEAM_SET_POINT != which_command_oneof_)
      {
        init_command_oneof(FieldNumber::SET_STEAM_SET_POINT);
      }
      command_oneof_.set_steam_set_point_ = value;
    }
    inline SetSteamSetPoint& mutable_set_steam_set_point()
    {
      if(FieldNumber::SET_STEAM_SET_POINT != which_command_oneof_)
      {
        init_command_oneof(FieldNumber::SET_STEAM_SET_POINT);
      }
      return command_oneof_.set_steam_set_point_;
    }
    inline const SetSteamSetPoint& get_set_steam_set_point() const { return command_oneof_.set_steam_set_point_; }
    inline const SetSteamSetPoint& set_steam_set_point() const { return command_oneof_.set_steam_set_point_; }

    static constexpr char const* SET_BOILER_SET_POINT_NAME = "set_boiler_set_point";
    inline bool has_set_boiler_set_point() const
    {
      return FieldNumber::SET_BOILER_SET_POINT == which_command_oneof_;
    }
    inline void clear_set_boiler_set_point()
    {
      if(FieldNumber::SET_BOILER_SET_POINT == which_command_oneof_)
      {
        which_command_oneof_ = FieldNumber::NOT_SET;
        command_oneof_.set_boiler_set_point_.~SetBoilerSetPoint();
      }
    }
    inline void set_set_boiler_set_point(const SetBoilerSetPoint& value)
    {
      if(FieldNumber::SET_BOILER_SET_POINT != which_command_oneof_)
      {
        init_command_oneof(FieldNumber::SET_BOILER_SET_POINT);
      }
      command_oneof_.set_boiler_set_point_ = value;
    }
    inline void set_set_boiler_set_point(const SetBoilerSetPoint&& value)
    {
      if(FieldNumber::SET_BOILER_SET_POINT != which_command_oneof_)
      {
        init_command_oneof(FieldNumber::SET_BOILER_SET_POINT);
      }
      command_oneof_.set_boiler_set_point_ = value;
    }
    inline SetBoilerSetPoint& mutable_set_boiler_set_point()
    {
      if(FieldNumber::SET_BOILER_SET_POINT != which_command_oneof_)
      {
        init_command_oneof(FieldNumber::SET_BOILER_SET_POINT);
      }
      return command_oneof_.set_boiler_set_point_;
    }
    inline const SetBoilerSetPoint& get_set_boiler_set_point() const { return command_oneof_.set_boiler_set_point_; }
    inline const SetBoilerSetPoint& set_boiler_set_point() const { return command_oneof_.set_boiler_set_point_; }


    ::EmbeddedProto::Error serialize(::EmbeddedProto::WriteBufferInterface& buffer) const override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;

      if(::EmbeddedProto::Error::NO_ERRORS == return_value)
      {
        return_value = request_id_.serialize_with_id(static_cast<uint32_t>(FieldNumber::REQUEST_ID), buffer, false);
      }

      switch(which_command_oneof_)
      {
        case FieldNumber::POWER_ON:
          if(has_power_on() && (::EmbeddedProto::Error::NO_ERRORS == return_value))
          {
            return_value = command_oneof_.power_on_.serialize_with_id(static_cast<uint32_t>(FieldNumber::POWER_ON), buffer, true);
          }
          break;

        case FieldNumber::POWER_OFF:
          if(has_power_off() && (::EmbeddedProto::Error::NO_ERRORS == return_value))
          {
            return_value = command_oneof_.power_off_.serialize_with_id(static_cast<uint32_t>(FieldNumber::POWER_OFF), buffer, true);
          }
          break;

        case FieldNumber::START_BREW:
          if(has_start_brew() && (::EmbeddedProto::Error::NO_ERRORS == return_value))
          {
            return_value = command_oneof_.start_brew_.serialize_with_id(static_cast<uint32_t>(FieldNumber::START_BREW), buffer, true);
          }
          break;

        case FieldNumber::STOP_BREW:
          if(has_stop_brew() && (::EmbeddedProto::Error::NO_ERRORS == return_value))
          {
            return_value = command_oneof_.stop_brew_.serialize_with_id(static_cast<uint32_t>(FieldNumber::STOP_BREW), buffer, true);
          }
          break;

        case FieldNumber::START_STEAM:
          if(has_start_steam() && (::EmbeddedProto::Error::NO_ERRORS == return_value))
          {
            return_value = command_oneof_.start_steam_.serialize_with_id(static_cast<uint32_t>(FieldNumber::START_STEAM), buffer, true);
          }
          break;

        case FieldNumber::STOP_STEAM:
          if(has_stop_steam() && (::EmbeddedProto::Error::NO_ERRORS == return_value))
          {
            return_value = command_oneof_.stop_steam_.serialize_with_id(static_cast<uint32_t>(FieldNumber::STOP_STEAM), buffer, true);
          }
          break;

        case FieldNumber::SET_STEAM_SET_POINT:
          if(has_set_steam_set_point() && (::EmbeddedProto::Error::NO_ERRORS == return_value))
          {
            return_value = command_oneof_.set_steam_set_point_.serialize_with_id(static_cast<uint32_t>(FieldNumber::SET_STEAM_SET_POINT), buffer, true);
          }
          break;

        case FieldNumber::SET_BOILER_SET_POINT:
          if(has_set_boiler_set_point() && (::EmbeddedProto::Error::NO_ERRORS == return_value))
          {
            return_value = command_oneof_.set_boiler_set_point_.serialize_with_id(static_cast<uint32_t>(FieldNumber::SET_BOILER_SET_POINT), buffer, true);
          }
          break;

        default:
          break;
      }

      return return_value;
    };

    ::EmbeddedProto::Error deserialize(::EmbeddedProto::ReadBufferInterface& buffer) override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;
      ::EmbeddedProto::WireFormatter::WireType wire_type = ::EmbeddedProto::WireFormatter::WireType::VARINT;
      uint32_t id_number = 0;
      FieldNumber id_tag = FieldNumber::NOT_SET;

      ::EmbeddedProto::Error tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
      while((::EmbeddedProto::Error::NO_ERRORS == return_value) && (::EmbeddedProto::Error::NO_ERRORS == tag_value))
      {
        id_tag = static_cast<FieldNumber>(id_number);
        switch(id_tag)
        {
          case FieldNumber::REQUEST_ID:
            return_value = request_id_.deserialize_check_type(buffer, wire_type);
            break;

          case FieldNumber::POWER_ON:
          case FieldNumber::POWER_OFF:
          case FieldNumber::START_BREW:
          case FieldNumber::STOP_BREW:
          case FieldNumber::START_STEAM:
          case FieldNumber::STOP_STEAM:
          case FieldNumber::SET_STEAM_SET_POINT:
          case FieldNumber::SET_BOILER_SET_POINT:
            return_value = deserialize_command_oneof(id_tag, buffer, wire_type);
            break;

          case FieldNumber::NOT_SET:
            return_value = ::EmbeddedProto::Error::INVALID_FIELD_ID;
            break;

          default:
            return_value = skip_unknown_field(buffer, wire_type);
            break;
        }

        if(::EmbeddedProto::Error::NO_ERRORS == return_value)
        {
          // Read the next tag.
          tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
        }
      }

      // When an error was detect while reading the tag but no other errors where found, set it in the return value.
      if((::EmbeddedProto::Error::NO_ERRORS == return_value)
         && (::EmbeddedProto::Error::NO_ERRORS != tag_value)
         && (::EmbeddedProto::Error::END_OF_BUFFER != tag_value)) // The end of the buffer is not an array in this case.
      {
        return_value = tag_value;
      }

      return return_value;
    };

    void clear() override
    {
      clear_request_id();
      clear_command_oneof();

    }

    static char const* field_number_to_name(const FieldNumber fieldNumber)
    {
      char const* name = nullptr;
      switch(fieldNumber)
      {
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
        case FieldNumber::START_STEAM:
          name = START_STEAM_NAME;
          break;
        case FieldNumber::STOP_STEAM:
          name = STOP_STEAM_NAME;
          break;
        case FieldNumber::SET_STEAM_SET_POINT:
          name = SET_STEAM_SET_POINT_NAME;
          break;
        case FieldNumber::SET_BOILER_SET_POINT:
          name = SET_BOILER_SET_POINT_NAME;
          break;
        default:
          name = "Invalid FieldNumber";
          break;
      }
      return name;
    }

#ifdef MSG_TO_STRING

    ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str) const
    {
      return this->to_string(str, 0, nullptr, true);
    }

    ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str, const uint32_t indent_level, char const* name, const bool first_field) const override
    {
      ::EmbeddedProto::string_view left_chars = str;
      int32_t n_chars_used = 0;

      if(!first_field)
      {
        // Add a comma behind the previous field.
        n_chars_used = snprintf(left_chars.data, left_chars.size, ",\n");
        if(0 < n_chars_used)
        {
          // Update the character pointer and characters left in the array.
          left_chars.data += n_chars_used;
          left_chars.size -= n_chars_used;
        }
      }

      if(nullptr != name)
      {
        if( 0 == indent_level)
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "\"%s\": {\n", name);
        }
        else
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s\"%s\": {\n", indent_level, " ", name);
        }
      }
      else
      {
        if( 0 == indent_level)
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "{\n");
        }
        else
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s{\n", indent_level, " ");
        }
      }
      
      if(0 < n_chars_used)
      {
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }

      left_chars = request_id_.to_string(left_chars, indent_level + 2, REQUEST_ID_NAME, true);
      left_chars = to_string_command_oneof(left_chars, indent_level + 2, false);
  
      if( 0 == indent_level) 
      {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "\n}");
      }
      else 
      {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "\n%*s}", indent_level, " ");
      }

      if(0 < n_chars_used)
      {
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }

      return left_chars;
    }

#endif // End of MSG_TO_STRING

  private:


      ::EmbeddedProto::FieldString<Command_request_id_LENGTH> request_id_;

      FieldNumber which_command_oneof_ = FieldNumber::NOT_SET;
      union command_oneof
      {
        command_oneof() {}
        ~command_oneof() {}
        PowerOn power_on_;
        PowerOff power_off_;
        StartBrew start_brew_;
        StopBrew stop_brew_;
        StartSteam start_steam_;
        StopSteam stop_steam_;
        SetSteamSetPoint set_steam_set_point_;
        SetBoilerSetPoint set_boiler_set_point_;
      };
      command_oneof command_oneof_;

      void init_command_oneof(const FieldNumber field_id)
      {
        if(FieldNumber::NOT_SET != which_command_oneof_)
        {
          // First delete the old object in the oneof.
          clear_command_oneof();
        }

        // C++11 unions only support nontrivial members when you explicitly call the placement new statement.
        switch(field_id)
        {
          case FieldNumber::POWER_ON:
            new(&command_oneof_.power_on_) PowerOn;
            break;

          case FieldNumber::POWER_OFF:
            new(&command_oneof_.power_off_) PowerOff;
            break;

          case FieldNumber::START_BREW:
            new(&command_oneof_.start_brew_) StartBrew;
            break;

          case FieldNumber::STOP_BREW:
            new(&command_oneof_.stop_brew_) StopBrew;
            break;

          case FieldNumber::START_STEAM:
            new(&command_oneof_.start_steam_) StartSteam;
            break;

          case FieldNumber::STOP_STEAM:
            new(&command_oneof_.stop_steam_) StopSteam;
            break;

          case FieldNumber::SET_STEAM_SET_POINT:
            new(&command_oneof_.set_steam_set_point_) SetSteamSetPoint;
            break;

          case FieldNumber::SET_BOILER_SET_POINT:
            new(&command_oneof_.set_boiler_set_point_) SetBoilerSetPoint;
            break;

          default:
            break;
         }

         which_command_oneof_ = field_id;
      }

      void clear_command_oneof()
      {
        switch(which_command_oneof_)
        {
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
          case FieldNumber::START_STEAM:
            ::EmbeddedProto::destroy_at(&command_oneof_.start_steam_);
            break;
          case FieldNumber::STOP_STEAM:
            ::EmbeddedProto::destroy_at(&command_oneof_.stop_steam_);
            break;
          case FieldNumber::SET_STEAM_SET_POINT:
            ::EmbeddedProto::destroy_at(&command_oneof_.set_steam_set_point_);
            break;
          case FieldNumber::SET_BOILER_SET_POINT:
            ::EmbeddedProto::destroy_at(&command_oneof_.set_boiler_set_point_);
            break;
          default:
            break;
        }
        which_command_oneof_ = FieldNumber::NOT_SET;
      }

      ::EmbeddedProto::Error deserialize_command_oneof(const FieldNumber field_id, 
                                    ::EmbeddedProto::ReadBufferInterface& buffer,
                                    const ::EmbeddedProto::WireFormatter::WireType wire_type)
      {
        ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;
        
        if(field_id != which_command_oneof_)
        {
          init_command_oneof(field_id);
        }

        switch(which_command_oneof_)
        {
          case FieldNumber::POWER_ON:
            return_value = command_oneof_.power_on_.deserialize_check_type(buffer, wire_type);
            break;
          case FieldNumber::POWER_OFF:
            return_value = command_oneof_.power_off_.deserialize_check_type(buffer, wire_type);
            break;
          case FieldNumber::START_BREW:
            return_value = command_oneof_.start_brew_.deserialize_check_type(buffer, wire_type);
            break;
          case FieldNumber::STOP_BREW:
            return_value = command_oneof_.stop_brew_.deserialize_check_type(buffer, wire_type);
            break;
          case FieldNumber::START_STEAM:
            return_value = command_oneof_.start_steam_.deserialize_check_type(buffer, wire_type);
            break;
          case FieldNumber::STOP_STEAM:
            return_value = command_oneof_.stop_steam_.deserialize_check_type(buffer, wire_type);
            break;
          case FieldNumber::SET_STEAM_SET_POINT:
            return_value = command_oneof_.set_steam_set_point_.deserialize_check_type(buffer, wire_type);
            break;
          case FieldNumber::SET_BOILER_SET_POINT:
            return_value = command_oneof_.set_boiler_set_point_.deserialize_check_type(buffer, wire_type);
            break;
          default:
            break;
        }

        if(::EmbeddedProto::Error::NO_ERRORS != return_value)
        {
          clear_command_oneof();
        }
        return return_value;
      }

#ifdef MSG_TO_STRING 
      ::EmbeddedProto::string_view to_string_command_oneof(::EmbeddedProto::string_view& str, const uint32_t indent_level, const bool first_field) const
      {
        ::EmbeddedProto::string_view left_chars = str;

        switch(which_command_oneof_)
        {
          case FieldNumber::POWER_ON:
            left_chars = command_oneof_.power_on_.to_string(left_chars, indent_level, POWER_ON_NAME, first_field);
            break;
          case FieldNumber::POWER_OFF:
            left_chars = command_oneof_.power_off_.to_string(left_chars, indent_level, POWER_OFF_NAME, first_field);
            break;
          case FieldNumber::START_BREW:
            left_chars = command_oneof_.start_brew_.to_string(left_chars, indent_level, START_BREW_NAME, first_field);
            break;
          case FieldNumber::STOP_BREW:
            left_chars = command_oneof_.stop_brew_.to_string(left_chars, indent_level, STOP_BREW_NAME, first_field);
            break;
          case FieldNumber::START_STEAM:
            left_chars = command_oneof_.start_steam_.to_string(left_chars, indent_level, START_STEAM_NAME, first_field);
            break;
          case FieldNumber::STOP_STEAM:
            left_chars = command_oneof_.stop_steam_.to_string(left_chars, indent_level, STOP_STEAM_NAME, first_field);
            break;
          case FieldNumber::SET_STEAM_SET_POINT:
            left_chars = command_oneof_.set_steam_set_point_.to_string(left_chars, indent_level, SET_STEAM_SET_POINT_NAME, first_field);
            break;
          case FieldNumber::SET_BOILER_SET_POINT:
            left_chars = command_oneof_.set_boiler_set_point_.to_string(left_chars, indent_level, SET_BOILER_SET_POINT_NAME, first_field);
            break;
          default:
            break;
        }

        return left_chars;
      }

#endif // End of MSG_TO_STRING
};

#endif // API_H