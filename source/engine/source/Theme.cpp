#include <engine/Theme.hpp>

Theme::Theme(Ref<Painter>& p)
{
	_standard_font_size = 16;
	_button_font_size = 20;
	_textbox_font_size = 20;
	_window_corner_radius = 2;
	_window_header_height = 30;
	_window_drop_shadow_size = 10;
	_button_corner_radius = 2;

	_drop_shadow = Color(0, 128);
	_ransparent = Color(0, 0);
	_border_dark = Color(29, 255);
	_border_light = Color(92, 255);
	_border_medium = Color(35, 255);
	_text_color = Color(255, 160);
	_disabled_text_color = Color(255, 80);
	_text_color_shadow = Color(0, 160);
	_icon_color = _text_color;

	_button_gradient_top_focused = Color(64, 255);
	_button_dragient_bot_focused = Color(48, 255);
	_button_gradient_top_unfocused = Color(74, 255);
	_button_gradient_bot_unfocused = Color(58, 255);
	_button_gradient_top_pushed = Color(41, 255);
	_button_gradient_bot_pushed = Color(29, 255);

	_window_fill_unfocused = Color(43, 230);
	_window_fill_focused = Color(45, 230);
	_window_title_unfocused = Color(220, 160);
	_window_title_focused = Color(255, 190);

	_window_header_gradient_top = _button_gradient_top_unfocused;
	_window_header_gradient_bot = _button_gradient_bot_unfocused;
	_window_header_sep_top = _border_light;
	_window_header_sep_bot = _border_dark;

	_window_popup = Color(50, 255);
	_window_popup_transparent = Color(50, 0);

	_font_normal = p->create_font("sans", "Roboto-Regular.ttf");
	_font_bold = p->create_font("sans-bold", "Roboto-Regular.ttf");
	_font_icons = p->create_font("icons", "entypo.ttf");
}
