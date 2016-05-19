#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>

#include "Handle.hpp"

template<GLsizei Size>
class ENGINE_API Textures : public Handles<Size>
{
public:
	Textures() : Handles<Size>()
	{
		glGenTextures(Size, &Handles<Size>::_ids[0]);
	}

	virtual ~Textures()
	{
		glDeleteTextures(Size, &Handles<Size>::_ids[0]);
	}

	Textures(Textures<Size> const& other) = delete;

	Textures(Textures<Size>&& other) : Handles<Size>(std::move(other))
	{ }

	Textures<Size>& operator=(Textures<Size> const& other) = delete;

	Textures<Size>& operator=(Textures<Size>&& other)
	{
		Handles<Size>::operator=(std::move(other));
		return *this;
	}
};

using Texture = Textures<1>;

extern ENGINE_API std::vector<unsigned char> load_texture(std::string const& filename, glm::ivec2& size, int& components);
