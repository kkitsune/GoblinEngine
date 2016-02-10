#pragma once

#include "Handle.hpp"

template<GLsizei Size>
class ENGINE_API Samplers : public Handles<Size>
{
public:
	Samplers() : Handles<Size>()
	{
		glGenSamplers(Size, &Handles<Size>::_ids[0]);
	}

	virtual ~Samplers()
	{
		glDeleteSamplers(Size, &Handles<Size>::_ids[0]);
	}

	Samplers(Samplers<Size> const& other) = delete;

	Samplers(Samplers<Size>&& other) : Handles<Size>(std::move(other))
	{ }

	Samplers<Size>& operator=(Samplers<Size> const& other) = delete;

	Samplers<Size>& operator=(Samplers<Size>&& other)
	{
		Handles<Size>::operator=(std::move(other));
		return *this;
	}
};

using Sampler = Samplers<1>;
