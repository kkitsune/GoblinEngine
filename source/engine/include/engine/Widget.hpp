#pragma once

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

	Widget* parent()
	{ return _parent; }

	Widget const* parent() const
	{ return _parent; }

	void parent(Widget* parent)
	{ _parent = parent; }

	Layout* layout()
	{ return _layout; }

	Layout const* layout() const
	{ return _layout.get(); }

	void layout(Layout* layout)
	{ _layout = layout; }

	Theme* theme()
	{ return _theme; }

	Theme const* theme() const
	{ return _theme.get(); }

	void theme(Theme* theme)
	{ _theme = theme; }

	ivec2 position() const
	{ return _pos; }

	void position(ivec2 const& pos)
	{ _pos = pos; }

	ivec2 absolute_position() const
	{ return _parent ? _parent->absolute_position() + _pos : _pos; }

	ivec2 size() const
	{ return _size; }

	void size(ivec2 const& size)
	{ _size = size; }

	ivec2::value_type width() const
	{ return _size.x; }

	void width(ivec2::value_type x)
	{ _size.x = x; }

	ivec2::value_type height() const
	{ return _size.y; }

	void height(ivec2::value_type y)
	{ _size.y = y; }

	ivec2 fixed_size() const
	{ return _fixed_size; }

	void fixed_size(ivec2 const& size)
	{ _fixed_size = size; }

	ivec2::value_type fixed_width() const
	{ return _fixed_size.x; }

	void fixed_width(ivec2::value_type x)
	{ _fixed_size.x = x; }

	ivec2::value_type fixed_height() const
	{ return _fixed_size.y; }

	void fixed_height(ivec2::value_type y)
	{ _fixed_size.y = y; }

	bool visible() const
	{ return _visible; }

	void visible(bool value)
	{ _visible = value; }

	bool visible_recursive() const
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

	uint64 child_count() const
	{ return _children.size(); }

	std::vector<Widget*> const& children() const
	{ return _children; }

	void add_child(Widget* child);

	void remove_child(uint64 index);

	void remove_child(Widget const* child);

	//Window* window();

	std::string id() const
	{ return _id; }

	void id(std::string const& id)
	{ _id = id; }

	bool enabled() const
	{ return _enabled; }

	void enabled(bool value)
	{ _enabled = value; }

	bool focused() const
	{ return _focused; }

	void focused(bool value)
	{ _focused = value; }

	void request_focus();

	std::string tooltip() const
	{ return _tooltip; }

	void tooltip(std::string const& tip)
	{ _tooltip = tip; }

	int font_size() const;

	void font_size(int size)
	{ _font_size = size; }

	bool has_font_size() const
	{ return _font_size > 0; }

	Cursor cursor() const
	{ return _cursor; }

	void cursor(Cursor cursor)
	{ _cursor = cursor; }

	bool contains(ivec2 const& p) const
	{
		ivec2 d = p - _pos;
		return d.x >= 0 && d.y >= 0 && d.x < _size.x && d.y < _size.y;
	}

	Widget* find_widget(ivec2 const& p);

	virtual bool mouse_button_event(ivec2 const& p, int button, bool down, int modifiers);

	virtual bool mouse_motion_event(ivec2 const& p, const ivec2& rel, int button, int modifiers);

	virtual bool mouse_drag_event(ivec2 const& p, const ivec2& rel, int button, int modifiers);

	virtual bool mouse_enter_event(ivec2 const& p, bool enter);

	virtual bool scroll_event(ivec2 const& p, ivec2 const& rel);

	virtual bool focus_event(bool focused);

	virtual bool keyboard_event(int key, int scancode, int action, int modifiers);

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
