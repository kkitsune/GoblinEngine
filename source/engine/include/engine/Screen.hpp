#pragma once

#include "Monitor.hpp"
#include "Widget.hpp"

class ENGINE_API Screen : public Widget
{
public:
	Screen(ivec2 const& size, std::string const& caption, bool resizable = true, Screen* share = nullptr);

	Screen(std::string const& caption, Monitor const& monitor, Screen* share = nullptr);

	virtual ~Screen();

	Screen(Screen const& other) = delete;

	Screen(Screen&& other) = default;

	Screen& operator=(Screen const& other) = delete;

	Screen& operator=(Screen&& other) = default;

	virtual void initialize();

	virtual std::string caption() const
	{ return _caption; }

	virtual void caption(std::string const& caption);

	virtual bool should_close() const;

	virtual void should_close(bool value);

	virtual vec3 backbround() const
	{ return _background; }

	virtual void background(vec3 const& color)
	{ _background = color; }

	virtual bool visible() const override
	{ return Widget::visible(); }

	virtual void visible(bool value) override;

	virtual ivec2 size() const override
	{ return Widget::size(); }

	virtual void size(ivec2 const& size) override;

	virtual ivec2::value_type width() const override
	{ return Widget::width(); }

	virtual void width(ivec2::value_type x) override;

	virtual ivec2::value_type height() const override
	{ return Widget::height(); }

	virtual void height(ivec2::value_type y) override;

	virtual ivec2 mouse_pos() const
	{ return _mouse_pos; }

	virtual Ref<Painter> painter() const
	{ return _painter; }

	virtual void draw_all();

	virtual void draw_contents()
	{ }

	virtual bool drop_event(std::vector<std::string> const& filenames);

	virtual bool keyboard_event(Key code, int scancode, Action action, int mods) override;

	virtual bool keyboard_character_event(uint32 codepoint) override;

	virtual bool resize_event(ivec2 const& size);

	bool cursor_pos_callback(double x, double y);

	bool mouse_button_callback(Mouse button, Action action, int mods);

	bool key_callback(Key code, int scancode, Action action, int mods);

	bool char_callback(uint32 codepoint);

	bool drop_callback(int count, char const** filenames);

	bool scroll_callback(double x, double y);

	bool resize_callback(int width, int height);

	void update_focus(Widget* widget);

	//void dispose_window(Window* window);

	//void center_window(WIndow* window);

	//void move_window_to_front(Window* window);

	void draw_widgets();

protected:
	std::vector<Widget*> _focus_path;
	ivec2 _fb_size;
	float _pixel_ratio;
	int _mouse_state, _modifiers;
	ivec2 _mouse_pos;
	bool _drag_active;
	Widget* _drag_widget = nullptr;
	double _last_interaction;
	bool _process_events;
	vec3 _background;
	std::string _caption;

	Ref<Painter> _painter;

private:
	void setup_events();

	struct GLFWcursor* _cursors[static_cast<int>(Cursor::Count)];
	struct GLFWwindow* _wnd;
};
