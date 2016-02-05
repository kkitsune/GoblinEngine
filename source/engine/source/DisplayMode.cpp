#include <engine/DisplayMode.hpp>
#include <GLFW/glfw3.h>

DisplayMode::DisplayMode()
{
	_width = 960;
	_height = 640;
	_refresh_rate = 60;
	_red = _green = _blue = 24;
}

DisplayMode::DisplayMode(GLFWvidmode const* mode)
{
	_width = mode->width;
	_height = mode->height;
	_red = mode->redBits;
	_green = mode->greenBits;
	_blue = mode->blueBits;
	_refresh_rate = mode->refreshRate;
}

DisplayMode::DisplayMode(int width, int height) : DisplayMode()
{
	_width = width;
	_height = height;
}

DisplayMode::DisplayMode(int width, int height, int refresh) : DisplayMode()
{
	_width = width;
	_height = height;
	_refresh_rate = refresh;
}

DisplayMode::DisplayMode(int width, int height, int red, int green, int blue) : DisplayMode()
{
	_width = width;
	_height = height;
	_red = red;
	_green = green;
	_blue = blue;
}

DisplayMode::DisplayMode(int width, int height, int red, int green, int blue, int refresh) : DisplayMode()
{
	_width = width;
	_height = height;
	_red = red;
	_green = green;
	_blue = blue;
	_refresh_rate = refresh;
}

DisplayMode::DisplayMode(DisplayMode const& other)
	: _width(other._width), _height(other._height), _red(other._red), _green(other._green), _blue(other._blue), _refresh_rate(other._refresh_rate)
{ }

DisplayMode::DisplayMode(DisplayMode&& other)
	: _width(other._width), _height(other._height), _red(other._red), _green(other._green), _blue(other._blue), _refresh_rate(other._refresh_rate)
{
	other._width = 960;
	other._height = 640;
	other._refresh_rate = 60;
	other._red = other._green = other._blue = 24;
}

DisplayMode& DisplayMode::operator=(DisplayMode const& other)
{
	_width = other.width();
	_height = other.height();
	_red = other.red_bits();
	_green = other.green_bits();
	_blue = other.blue_bits();
	_refresh_rate = other.refresh_rate();

	return *this;
}

DisplayMode& DisplayMode::operator=(DisplayMode&& other)
{
	_width = other.width();
	_height = other.height();
	_red = other.red_bits();
	_green = other.green_bits();
	_blue = other.blue_bits();
	_refresh_rate = other.refresh_rate();

	other._width = 960;
	other._height = 640;
	other._refresh_rate = 60;
	other._red = other._green = other._blue = 24;

	return *this;
}

int DisplayMode::width() const
{
	return _width;
}

void DisplayMode::width(int value)
{
	_width = value;
}

int DisplayMode::height() const
{
	return _height;
}

void DisplayMode::height(int value)
{
	_height = value;
}

int DisplayMode::red_bits() const
{
	return _red;
}

void DisplayMode::red_bits(int value)
{
	_red = value;
}

int DisplayMode::green_bits() const
{
	return _green;
}

void DisplayMode::green_bits(int value)
{
	_green = value;
}

int DisplayMode::blue_bits() const
{
	return _blue;
}

void DisplayMode::blue_bits(int value)
{
	_blue = value;
}

int DisplayMode::refresh_rate() const
{
	return _refresh_rate;
}

void DisplayMode::refresh_rate(int value)
{
	_refresh_rate = value;
}
