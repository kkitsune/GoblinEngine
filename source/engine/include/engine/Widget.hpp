#pragma once

#include "InputEnums.hpp"
#include "Layout.hpp"
#include "Theme.hpp"

class Widget : public Object
{
public:
	Widget(Widget* parent);

	Widget(Widget const& other) = delete;

	Widget(Widget&& other) = default;

	Widget& operator=(Widget const& other) = delete;

	Widget& operator=(Widget&& other) = default;

	virtual Widget* parent()
	{ return _parent; }

	virtual Widget const* parent() const
	{ return _parent; }

	virtual void parent(Widget* parent)
	{ _parent = parent; }

	virtual Layout* layout()
	{ return _layout; }

	virtual Layout const* layout() const
	{ return _layout.get(); }

	virtual void layout(Layout* layout)
	{ _layout = layout; }

	virtual Theme* theme()
	{ return _theme; }

	virtual Theme const* theme() const
	{ return _theme.get(); }

	virtual void theme(Theme* theme)
	{ _theme = theme; }

	virtual ivec2 position() const
	{ return _pos; }

	virtual void position(ivec2 const& pos)
	{ _pos = pos; }

	virtual ivec2 absolute_position() const
	{ return _parent ? _parent->absolute_position() + _pos : _pos; }

	virtual ivec2 size() const
	{ return _size; }

	virtual void size(ivec2 const& size)
	{ _size = size; }

	virtual ivec2::value_type width() const
	{ return _size.x; }

	virtual void width(ivec2::value_type x)
	{ _size.x = x; }

	virtual ivec2::value_type height() const
	{ return _size.y; }

	virtual void height(ivec2::value_type y)
	{ _size.y = y; }

	virtual ivec2 fixed_size() const
	{ return _fixed_size; }

	virtual void fixed_size(ivec2 const& size)
	{ _fixed_size = size; }

	virtual ivec2::value_type fixed_width() const
	{ return _fixed_size.x; }

	virtual void fixed_width(ivec2::value_type x)
	{ _fixed_size.x = x; }

	virtual ivec2::value_type fixed_height() const
	{ return _fixed_size.y; }

	virtual void fixed_height(ivec2::value_type y)
	{ _fixed_size.y = y; }

	virtual bool visible() const
	{ return _visible; }

	virtual void visible(bool value)
	{ _visible = value; }

	virtual bool visible_recursive() const
	{
		bool visible = true;
		Widget const* w = this;
		while (w)
		{
			visible &= w->visible();
			w = w->parent();
		}
		return visible;
	}

	virtual uint64 child_count() const
	{ return _children.size(); }

	virtual std::vector<Widget*> const& children() const
	{ return _children; }

	virtual void add_child(Widget* child);

	virtual void remove_child(uint64 index);

	virtual void remove_child(Widget const* child);

	//Window* window();

	virtual std::string id() const
	{ return _id; }

	virtual void id(std::string const& id)
	{ _id = id; }

	virtual bool enabled() const
	{ return _enabled; }

	virtual void enabled(bool value)
	{ _enabled = value; }

	virtual bool focused() const
	{ return _focused; }

	virtual void focused(bool value)
	{ _focused = value; }

	virtual void request_focus();

	virtual std::string tooltip() const
	{ return _tooltip; }

	virtual void tooltip(std::string const& tip)
	{ _tooltip = tip; }

	virtual int font_size() const;

	virtual void font_size(int size)
	{ _font_size = size; }

	virtual bool has_font_size() const
	{ return _font_size > 0; }

	virtual Cursor cursor() const
	{ return _cursor; }

	virtual void cursor(Cursor cursor)
	{ _cursor = cursor; }

	virtual bool contains(ivec2 const& p) const
	{
		ivec2 d = p - _pos;
		return d.x >= 0 && d.y >= 0 && d.x < _size.x && d.y < _size.y;
	}

	virtual Widget* find_widget(ivec2 const& p);

	virtual bool mouse_button_event(ivec2 const& p, Mouse button, bool down, int modifiers);

	virtual bool mouse_motion_event(ivec2 const& p, const ivec2& rel, Mouse button, int modifiers);

	virtual bool mouse_drag_event(ivec2 const& p, const ivec2& rel, Mouse button, int modifiers);

	virtual bool mouse_enter_event(ivec2 const& p, bool enter);

	virtual bool scroll_event(ivec2 const& p, ivec2 const& rel);

	virtual bool focus_event(bool focused);

	virtual bool keyboard_event(Key key, int scancode, Action action, int modifiers);

	virtual bool keyboard_character_event(unsigned int codepoint);

	virtual ivec2 preferred_size(Ref<Painter> p);

	virtual void perform_layout(Ref<Painter> p);

	virtual void draw(Ref<Painter> p);

protected:
	virtual ~Widget();

	Widget* _parent;
	std::vector<Widget*> _children;

	Ref<Layout> _layout;
	Ref<Theme> _theme;

	ivec2 _pos, _size, _fixed_size;
	bool _visible, _enabled, _focused, _mouse_focus;
	std::string _id, _tooltip;
	int _font_size;
	Cursor _cursor;
};
