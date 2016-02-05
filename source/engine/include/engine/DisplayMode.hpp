#pragma once

#include <utility>

#include "Common.hpp"

class ENGINE_API DisplayMode
{
	friend class Monitor;

	friend class Screen;

public:
	DisplayMode();

	DisplayMode(int width, int height);

	DisplayMode(int width, int height, int refresh);

	DisplayMode(int width, int height, int red, int green, int blue);

	DisplayMode(int width, int height, int red, int green, int blue, int refresh);

	DisplayMode(DisplayMode const& other);

	DisplayMode(DisplayMode&& other);

	DisplayMode& operator=(DisplayMode const& other);

	DisplayMode& operator=(DisplayMode&& other);

	int width() const;

	void width(int value);

	int height() const;

	void height(int value);

	int red_bits() const;

	void red_bits(int value);

	int green_bits() const;

	void green_bits(int value);

	int blue_bits() const;

	void blue_bits(int value);

	int refresh_rate() const;

	void refresh_rate(int value);

private:
	DisplayMode(struct GLFWvidmode const* mode);

	int _width;
	int _height;
	int _red;
	int _green;
	int _blue;
	int _refresh_rate;
};
