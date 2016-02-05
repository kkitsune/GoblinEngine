#pragma once

#include "Monitor.hpp"
#include "Object.hpp"

class ENGINE_API Screen
{
public:
	Screen();

	virtual ~Screen();

	Screen(Screen const& other) = delete;

	Screen(Screen&& other);

	Screen& operator=(Screen const& other) = delete;

	Screen& operator=(Screen&& other);

private:
	struct GLFWwindow* _wnd;
};
