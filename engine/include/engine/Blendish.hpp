#pragma once

#include <engine/Painter.hpp>

class Blendish final
{
public:
	Blendish(Painter& painter);

	~Blendish();

	Blendish(Blendish const& other) = delete;

	Blendish(Blendish&& other) = default;

	Blendish& operator=(Blendish const& other) = delete;

	Blendish& operator=(Blendish&& other) = default;

private:
	struct NVGcontext* _vg;
};
