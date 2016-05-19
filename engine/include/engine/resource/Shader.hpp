#pragma once

#include "Handle.hpp"

class ENGINE_API Shader : public Handle
{
public:
	Shader();

	virtual ~Shader();

	Shader(Shader const& other) = delete;

	Shader(Shader&& other) : Handle(std::move(other))
	{ }

	Shader& operator=(Shader const& other) = delete;

	Shader& operator=(Shader&& other)
	{
		Handle::operator=(std::move(other));
		return *this;
	}
};

extern ENGINE_API void compile_shader(Shader& shader, std::string const& name);
