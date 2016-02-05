#include <engine/Widget.hpp>

Widget::Widget(Widget* parent) : _parent(nullptr), _layout(), _theme(),
								 _pos(), _size(), _fixed_size(), _visible(true), _enabled(true), _focused(false), _mouse_focus(false),
								 _tooltip(), _font_size(-1), _cursor(Cursor::Arrow)
{
	if (parent)
	{
		parent->add_child(this);
		_theme = parent->_theme;
	}
}

Widget::~Widget()
{
	for (auto child : _children)
		if (child) child->dec_ref();
}

void Widget::add_child(Widget* child)
{

}

void Widget::remove_child(uint64 index)
{

}

void Widget::remove_child(Widget const* child)
{

}

void Widget::request_focus()
{

}

int Widget::font_size() const
{
	return _font_size < 0 ? _theme->_standard_font_size : _font_size;
}

Widget* Widget::find_widget(ivec2 const& p)
{
	for (auto it = _children.rbegin(); it != _children.rend(); ++it)
	{
		Widget* child = *it;
		if (child->visible() && child->contains(p))
			return child->find_widget(p - _pos);
	}
	return contains(p) ? this : nullptr;
}

bool Widget::mouse_button_event(ivec2 const& p, int button, bool down, int modifiers)
{
	return false;
}

bool Widget::mouse_motion_event(ivec2 const& p, const ivec2& rel, int button, int modifiers)
{
	return false;
}

bool Widget::mouse_drag_event(ivec2 const& p, const ivec2& rel, int button, int modifiers)
{
	return false;
}

bool Widget::mouse_enter_event(ivec2 const& p, bool enter)
{
	return false;
}

bool Widget::scroll_event(ivec2 const& p, ivec2 const& rel)
{
	return false;
}

bool Widget::focus_event(bool focused)
{
	return false;
}

bool Widget::keyboard_event(int key, int scancode, int action, int modifiers)
{
	return false;
}

bool Widget::keyboard_character_event(unsigned int codepoint)
{
	return false;
}

ivec2 Widget::preferred_size(Ref<Painter> p)
{
	return _layout ? _layout->preferred_size(p, this) : _size;
}

void Widget::perform_layout(Ref<Painter> p)
{
	if (_layout)
		_layout->perform_layout(p, this);
	else
		for (auto child : _children)
		{
			ivec2 pref = child->preferred_size(p), fix = child->fixed_size();
			child->size(ivec2(fix.x ? fix.x : pref.x, fix.y ? fix.y : pref.y));
			child->perform_layout(p);
		}
}

void Widget::draw(Ref<Painter> p)
{

}
