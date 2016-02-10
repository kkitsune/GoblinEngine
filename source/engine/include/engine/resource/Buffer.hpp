#pragma once

#include "Handle.hpp"

template<GLsizei Size>
class ENGINE_API Buffers : public Handles<Size>
{
public:
	Buffers() : Handles<Size>()
	{
		glGenBuffers(Size, &Handles<Size>::_ids[0]);
	}

	virtual ~Buffers()
	{
		glDeleteBuffers(Size, &Handles<Size>::_ids[0]);
	}

	Buffers(Buffers const& other) = delete;

	Buffers(Buffers&& other) : Handles<Size>(std::move(other))
	{ }

	Buffers& operator=(Buffers const& other) = delete;

	Buffers& operator=(Buffers&& other)
	{
		Handles<Size>::operator=(std::move(other));
		return *this;
	}
};

using Buffer = Buffers<1>;

template<typename T>
inline void glBufferData(GLenum target, std::vector<T> const& data, GLenum usage)
{
	glBufferData(target, data.size() * sizeof(T), &data[0], usage);
}

template<typename T, std::size_t Size>
inline void glBufferData(GLenum target, std::array<T, Size> const& data, GLenum usage)
{
	glBufferData(target, Size * sizeof(T), &data[0], usage);
}
