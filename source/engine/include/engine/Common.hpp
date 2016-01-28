#pragma once

#include <engine/engine_api.h>
#include <glm/gtc/quaternion.hpp>
#include <glm/glm.hpp>
#include <string>
#include <vector>

using glm::int8;
using glm::int16;
using glm::int32;
using glm::int64;

using glm::uint8;
using glm::uint16;
using glm::uint32;
using glm::uint64;

using glm::ivec2;
using glm::ivec3;
using glm::ivec4;

using glm::vec2;
using glm::vec3;
using glm::vec4;

using glm::quat;

using glm::mat2x3;
using glm::mat4;

struct Color
{
	Color() = default;

	Color(Color const& other) = default;

	Color(Color&& other) = default;

	Color& operator=(Color const& other) = default;

	Color& operator=(Color&& other) = default;

	Color(vec3 const& v, float alpha = 1.f) : value(v, alpha)
	{ }

	Color(ivec3 const& v, int alpha = 255) : Color(v.r, v.g, v.b, alpha)
	{ }

	Color(vec4 const& v) : value(v)
	{ }

	Color(ivec4 const& v) : Color(v.r, v.g, v.b, v.a)
	{ }

	Color(float intensity, float alpha = 1.f) : Color(intensity, intensity, intensity, alpha)
	{ }

	Color(int intensity, int alpha = 255) : Color(intensity, intensity, intensity, alpha)
	{ }

	Color(float r, float g, float b, float a = 1.f) : value(r, g, b, a)
	{ }

	Color(int r, int g, int b, int a = 255) : value((float) r / 255.f, (float) g / 255.f, (float) b / 255.f, (float) a / 255.f)
	{ }

	operator vec4()
	{ return value; }

	vec4 value;
};

extern ENGINE_API std::string file_dialog(std::vector<std::pair<std::string, std::string>> const& filetypes, bool save);
