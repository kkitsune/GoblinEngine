#pragma once

#include <engine/config.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <array>

template<GLsizei Size>
class ENGINE_API Handles
{
	static_assert(Size > 0, "Size must be greater than 0");
public:
	Handles() : _ids()
	{ }

	virtual ~Handles()
	{ }

	Handles(Handles<Size> const& other) = delete;

	Handles(Handles<Size>&& other) : _ids(std::move(other._ids))
	{ other._ids.fill(0); }

	Handles<Size>& operator=(Handles<Size> const& other) = delete;

	Handles<Size>& operator=(Handles<Size>&& other)
	{
		_ids = std::move(other._ids);
		other._ids.fill(0);
		return *this;
	}

	typename std::array<GLuint, Size>::const_reference operator[](std::size_t index) const
	{ return _ids[index]; }

	operator GLuint() const
	{ return _ids[0]; }

protected:
	std::array<GLuint, Size> _ids;
};

using Handle = Handles<1>;
