#pragma once

#include "Handle.hpp"

#include <vector>

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

	template<typename T>
	static inline void data(GLenum target, std::vector<T> const& data, GLenum usage)
	{
		glBufferData(target, data.size() * sizeof(T), &data[0], usage);
	}

	template<typename T, std::size_t Length>
	static inline void data(GLenum target, std::array<T, Length> const& data, GLenum usage)
	{
		glBufferData(target, Size * sizeof(T), &data[0], usage);
	}
};

using Buffer = Buffers<1>;
