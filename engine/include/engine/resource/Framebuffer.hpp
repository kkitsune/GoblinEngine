#pragma once

#include "Handle.hpp"

template<GLsizei Size>
class ENGINE_API Framebuffers : public Handles<Size>
{
public:
	Framebuffers() : Handles<Size>()
	{
		glGenFramebuffers(Size, &Handles<Size>::_ids[0]);
	}

	virtual ~Framebuffers()
	{
		glDeleteFramebuffers(Size, &Handles<Size>::_ids[0]);
	}

	Framebuffers(Framebuffers<Size> const& other) = delete;

	Framebuffers(Framebuffers<Size>&& other) : Handles<Size>(std::move(other))
	{ }

	Framebuffers<Size>& operator=(Framebuffers<Size> const& other) = delete;

	Framebuffers<Size>& operator=(Framebuffers<Size>&& other)
	{
		Handles<Size>::operator=(std::move(other));
		return *this;
	}
};

using Framebuffer = Framebuffers<1>;
