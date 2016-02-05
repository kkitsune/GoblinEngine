#include <engine/Screen.hpp>

Screen::Screen() : _wnd(nullptr)
{ }

Screen::~Screen()
{
}

Screen::Screen(Screen&& other) : _wnd(other._wnd)
{
	other._wnd = nullptr;
}

Screen& Screen::operator=(Screen&& other)
{
	_wnd = other._wnd;
	other._wnd = nullptr;
	return *this;
}
