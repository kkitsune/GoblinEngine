#include <engine/Widget.hpp>
#include <engine/Screen.hpp>
#include <algorithm>

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
		if (child) child->unref();
}

void Widget::add_child(Widget* child)
{
	_children.push_back(child);
	child->ref();
	child->parent(this);
}

void Widget::remove_child(uint64 index)
{
	Widget* widget = _children[index];
	_children.erase(_children.begin() + index);
	widget->unref();
}

void Widget::remove_child(Widget const* child)
{
	_children.erase(std::remove(_children.begin(), _children.end(), child), _children.end());
	child->unref();
}

/*Window* Widget::window()
{
	Widget *widget = this;
	while (true)
	{
		if (!widget)
			throw std::runtime_error("Widget:internal error (could not find parent window)");
		Window *window = dynamic_cast<Window*>(widget);
		if (window)
			return window;
		widget = widget->parent();
	}
}*/

void Widget::request_focus()
{
	/*Widget* widget = this;
	while (widget->parent())
		widget = widget->parent();
	dynamic_cast<Screen*>(widget)->update_focus(this);*/
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

bool Widget::mouse_button_event(ivec2 const& p, Mouse button, bool down, int modifiers)
{
	for (auto it = _children.rbegin(); it != _children.rend(); ++it)
	{
		Widget* child = *it;
		if (child->visible() && child->contains(p - _pos) &&
			child->mouse_button_event(p - _pos, button, down, modifiers))
			return true;
	}
	if (button == Mouse::Left && down && !_focused)
		request_focus();
	return false;
}

bool Widget::mouse_motion_event(ivec2 const& p, const ivec2& rel, Mouse button, int modifiers)
{
	for (auto it = _children.rbegin(); it != _children.rend(); ++it)
	{
		Widget* child = *it;
		if (!child->visible())
			continue;
		bool contained = child->contains(p - _pos), prev_contained = child->contains(p - _pos - rel);
		if (contained != prev_contained)
			child->mouse_enter_event(p, contained);
		if ((contained || prev_contained) && child->mouse_motion_event(p - _pos, rel, button, modifiers))
			return true;
	}
	return false;
}

bool Widget::mouse_drag_event(ivec2 const&, const ivec2&, Mouse, int)
{
	return false;
}

bool Widget::mouse_enter_event(ivec2 const&, bool enter)
{
	_mouse_focus = enter;
	return false;
}

bool Widget::scroll_event(ivec2 const& p, ivec2 const& rel)
{
	for (auto it = _children.rbegin(); it != _children.rend(); ++it)
	{
		Widget* child = *it;
		if (!child->visible())
			continue;
		if (child->contains(p - _pos) && child->scroll_event(p - _pos, rel))
			return true;
	}
	return false;
}

bool Widget::focus_event(bool focused)
{
	_focused = focused;
	return false;
}

bool Widget::keyboard_event(Key, int, Action, int)
{
	return false;
}

bool Widget::keyboard_character_event(unsigned int)
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
	if (_children.empty())
		return;

	p->translate(vec2(_pos.x, _pos.y));
	for (auto child : _children)
		if (child->visible())
			child->draw(p);
	p->translate(vec2(-_pos.x, -_pos.y));
}
