#pragma once

#include "Painter.hpp"

class Layout : public Object
{
public:
	virtual void perform_layout(Ref<Painter> p, class Widget* widget) = 0;

	virtual ivec2 preferred_size(Ref<Painter> p, class Widget const& widget) = 0;

protected:
	virtual ~Layout()
	{ }
};
