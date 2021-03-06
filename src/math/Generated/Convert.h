
#pragma once

#include <math/Generated/Types.h>

#include <obj/Reflect/Meta.h>
#include <obj/Reflect/Enum.h>
#include <obj/String/StringConvert.h>

namespace mud
{
    template <> inline void from_string(const string& str, mud::Axis& val) { val = static_cast<mud::Axis>(enu<mud::Axis>().value(str.c_str())); };
    template <> inline void to_string(const mud::Axis& val, string& str) { str = enu<mud::Axis>().m_map[size_t(val)]; };
    
    template <> inline void from_string(const string& str, mud::Clockwise& val) { val = static_cast<mud::Clockwise>(enu<mud::Clockwise>().value(str.c_str())); };
    template <> inline void to_string(const mud::Clockwise& val, string& str) { str = enu<mud::Clockwise>().m_map[size_t(val)]; };
    
    template <> inline void from_string(const string& str, mud::Side& val) { val = static_cast<mud::Side>(enu<mud::Side>().value(str.c_str())); };
    template <> inline void to_string(const mud::Side& val, string& str) { str = enu<mud::Side>().m_map[size_t(val)]; };
    
    template <> inline void from_string(const string& str, mud::SignedAxis& val) { val = static_cast<mud::SignedAxis>(enu<mud::SignedAxis>().value(str.c_str())); };
    template <> inline void to_string(const mud::SignedAxis& val, string& str) { str = enu<mud::SignedAxis>().m_map[size_t(val)]; };
    
    template <> inline void from_string(const string& str, mud::SpectrumPalette& val) { val = static_cast<mud::SpectrumPalette>(enu<mud::SpectrumPalette>().value(str.c_str())); };
    template <> inline void to_string(const mud::SpectrumPalette& val, string& str) { str = enu<mud::SpectrumPalette>().m_map[size_t(val)]; };
    
    
    template <> inline void from_string(const string& str, mud::Colour& val) { string_to_fixed_vector<mud::Colour, float>(str, val); };
    template <> inline void to_string(const mud::Colour& val, string& str) { fixed_vector_to_string<mud::Colour, 4>(val, str); };
    
    template <> inline void from_string(const string& str, mud::ivec3& val) { string_to_fixed_vector<mud::ivec3, int>(str, val); };
    template <> inline void to_string(const mud::ivec3& val, string& str) { fixed_vector_to_string<mud::ivec3, 3>(val, str); };
    
    template <> inline void from_string(const string& str, mud::quat& val) { string_to_fixed_vector<mud::quat, float>(str, val); };
    template <> inline void to_string(const mud::quat& val, string& str) { fixed_vector_to_string<mud::quat, 4>(val, str); };
    
    template <> inline void from_string(const string& str, mud::uvec2& val) { string_to_fixed_vector<mud::uvec2, uint32_t>(str, val); };
    template <> inline void to_string(const mud::uvec2& val, string& str) { fixed_vector_to_string<mud::uvec2, 2>(val, str); };
    
    template <> inline void from_string(const string& str, mud::uvec3& val) { string_to_fixed_vector<mud::uvec3, uint32_t>(str, val); };
    template <> inline void to_string(const mud::uvec3& val, string& str) { fixed_vector_to_string<mud::uvec3, 3>(val, str); };
    
    template <> inline void from_string(const string& str, mud::uvec4& val) { string_to_fixed_vector<mud::uvec4, uint32_t>(str, val); };
    template <> inline void to_string(const mud::uvec4& val, string& str) { fixed_vector_to_string<mud::uvec4, 4>(val, str); };
    
    template <> inline void from_string(const string& str, mud::vec2& val) { string_to_fixed_vector<mud::vec2, float>(str, val); };
    template <> inline void to_string(const mud::vec2& val, string& str) { fixed_vector_to_string<mud::vec2, 2>(val, str); };
    
    template <> inline void from_string(const string& str, mud::vec3& val) { string_to_fixed_vector<mud::vec3, float>(str, val); };
    template <> inline void to_string(const mud::vec3& val, string& str) { fixed_vector_to_string<mud::vec3, 3>(val, str); };
    
    template <> inline void from_string(const string& str, mud::vec4& val) { string_to_fixed_vector<mud::vec4, float>(str, val); };
    template <> inline void to_string(const mud::vec4& val, string& str) { fixed_vector_to_string<mud::vec4, 4>(val, str); };
    
}
