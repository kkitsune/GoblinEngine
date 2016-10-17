#pragma once

#include <type_safe/floating_point.hpp>
#include <type_safe/narrow_cast.hpp>
#include <type_safe/integer.hpp>
#include <type_safe/boolean.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>

using type_safe::make_unsigned;
using type_safe::make_signed;
using type_safe::narrow_cast;
using type_safe::boolean;

using i8  = type_safe::integer<int8_t>;
using i16 = type_safe::integer<int16_t>;
using i32 = type_safe::integer<int32_t>;
using i64 = type_safe::integer<int64_t>;

using u8  = type_safe::integer<uint8_t>;
using u16 = type_safe::integer<uint16_t>;
using u32 = type_safe::integer<uint32_t>;
using u64 = type_safe::integer<uint64_t>;

#ifndef GOBLIN_USE_DOUBLE
using real = type_safe::floating_point<float>;
#else
using real = type_safe::floating_point<double>;
#endif

using glm::quat;
using glm::mat2;
using glm::mat3;
using glm::mat4;

using glm::vec2;
using glm::vec3;
using glm::vec4;

using glm::uvec2;
using glm::uvec3;
using glm::uvec4;

using glm::ivec2;
using glm::ivec3;
using glm::ivec4;

using glm::bvec2;
using glm::bvec3;
using glm::bvec4;

constexpr u8 operator ""_u8(unsigned long long value)
{ return {static_cast<uint8_t>(value)}; }

constexpr u16 operator ""_u16(unsigned long long value)
{ return {static_cast<uint16_t>(value)}; }

constexpr u32 operator ""_u32(unsigned long long value)
{ return {static_cast<uint32_t>(value)}; }

constexpr u64 operator ""_u64(unsigned long long value)
{ return {static_cast<uint64_t>(value)}; }

constexpr i8 operator ""_i8(unsigned long long value)
{ return {static_cast<int8_t>(value)}; }

constexpr i16 operator ""_i16(unsigned long long value)
{ return {static_cast<int16_t>(value)}; }

constexpr i32 operator ""_i32(unsigned long long value)
{ return {static_cast<int32_t>(value)}; }

constexpr i64 operator ""_i64(unsigned long long value)
{ return {static_cast<int64_t>(value)}; }
