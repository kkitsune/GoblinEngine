#pragma once

#include "Painter.hpp"

class Theme : public Object
{
public:
	Theme(Ref<Painter> p);

	Theme(Theme const& other) = default;

	Theme(Theme&& other) = default;

	Theme& operator=(Theme const& other) = default;

	Theme& operator=(Theme&& other) = default;

	int _font_normal;
	int _font_bold;
	int _font_icons;

	int _standard_font_size;
	int _button_font_size;
	int _textbox_font_size;
	int _window_corner_radius;
	int _window_header_height;
	int _window_drop_shadow_size;
	int _button_corner_radius;

	Color _drop_shadow;
	Color _ransparent;
	Color _border_dark;
	Color _border_light;
	Color _border_medium;
	Color _text_color;
	Color _disabled_text_color;
	Color _text_color_shadow;
	Color _icon_color;

	Color _button_gradient_top_focused;
	Color _button_dragient_bot_focused;
	Color _button_gradient_top_unfocused;
	Color _button_gradient_bot_unfocused;
	Color _button_gradient_top_pushed;
	Color _button_gradient_bot_pushed;

	Color _window_fill_unfocused;
	Color _window_fill_focused;
	Color _window_title_unfocused;
	Color _window_title_focused;

	Color _window_header_gradient_top;
	Color _window_header_gradient_bot;
	Color _window_header_sep_top;
	Color _window_header_sep_bot;

	Color _window_popup;
	Color _window_popup_transparent;

protected:
	virtual ~Theme()
	{ }
};
