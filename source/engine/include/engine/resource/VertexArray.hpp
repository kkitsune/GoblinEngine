#pragma once

#include "Handle.hpp"

template<GLsizei Size>
class ENGINE_API VertexArrays : public Handles<Size>
{
public:
	VertexArrays() : Handles<Size>()
	{
		glGenVertexArrays(Size, &Handles<Size>::_ids[0]);
	}

	virtual ~VertexArrays()
	{
		glDeleteVertexArrays(Size, &Handles<Size>::_ids[0]);
	}

	VertexArrays(VertexArrays const& other) = delete;

	VertexArrays(VertexArrays&& other) : Handles<Size>(std::move(other))
	{ }

	VertexArrays<Size>& operator=(VertexArrays<Size> const& other) = delete;

	VertexArrays<Size>& operator=(VertexArrays<Size>&& other)
	{
		Handles<Size>::operator=(std::move(other));
		return *this;
	}
};

using VertexArray = VertexArrays<1>;

inline void glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLsizei offset)
{
	glVertexAttribPointer(index, size, type, normalized, stride, reinterpret_cast<void const*>(offset));
}
