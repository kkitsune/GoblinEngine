#pragma once

#include "Monitor.hpp"
#include "Widget.hpp"

class ENGINE_API Screen : public Widget
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
