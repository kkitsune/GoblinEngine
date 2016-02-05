#pragma once

class Widget
{
public:
	Widget();

	virtual ~Widget();

	Widget(Widget const& other) = default;

	Widget(Widget&& other) = default;

	Widget& operator=(Widget const& other) = default;

	Widget& operator=(Widget&& other) = default;
};
