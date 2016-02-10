#include <engine/Screen.hpp>
#include <engine/Core.hpp>

#include <glbinding/Binding.h>
#include <GLFW/glfw3.h>

Screen::Screen(ivec2 const& size, std::string const& caption, bool resizable, Screen* share)
: Widget(nullptr), _caption(caption), _wnd(nullptr)
{
	memset(_cursors, 0, sizeof(GLFWcursor*) * static_cast<int>(Cursor::Count));
	Core::instance().reset_window_hints();

	glfwWindowHint(GLFW_VISIBLE, 0);
	glfwWindowHint(GLFW_RESIZABLE, resizable ? 1 : 0);

	_wnd = glfwCreateWindow(size.x, size.y, caption.c_str(), nullptr, share == nullptr ? nullptr : share->_wnd);
	glfwMakeContextCurrent(_wnd);
	glbinding::Binding::initialize(false);
	glfwSetWindowUserPointer(_wnd, this);

	glfwGetFramebufferSize(_wnd, &_fb_size.x, &_fb_size.y);
	glViewport(0, 0, _fb_size.x, _fb_size.y);
	glClearColor(_background.x, _background.y, _background.z, 1.f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_CCW);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glfwSwapInterval(0);
	glfwSwapBuffers(_wnd);

#if defined(__APPLE__)
	/* Poll for events once before starting a potentially
	   lengthy loading process. This is needed to be
	   classified as "interactive" by other software such
	   as iTerm2 */

	glfwPollEvents();
#endif

	setup_events();
	initialize();
}

Screen::Screen(std::string const& caption, Monitor const& monitor, Screen* share)
: Widget(nullptr), _caption(caption), _wnd(nullptr)
{
	memset(_cursors, 0, sizeof(GLFWcursor*) * static_cast<int>(Cursor::Count));
	Core::instance().reset_window_hints();

	glfwWindowHint(GLFW_VISIBLE, 0);

	auto mode = monitor.display_mode();
	glfwWindowHint(GLFW_RED_BITS, mode.red_bits());
	glfwWindowHint(GLFW_GREEN_BITS, mode.green_bits());
	glfwWindowHint(GLFW_BLUE_BITS, mode.blue_bits());
	glfwWindowHint(GLFW_REFRESH_RATE, mode.refresh_rate());

	_wnd = glfwCreateWindow(mode.width(), mode.height(), caption.c_str(), monitor._monitor, share == nullptr ? nullptr : share->_wnd);
	glfwMakeContextCurrent(_wnd);
	glbinding::Binding::initialize(false);
	glfwSetWindowUserPointer(_wnd, this);

	glfwGetFramebufferSize(_wnd, &_fb_size.x, &_fb_size.y);
	glViewport(0, 0, _fb_size.x, _fb_size.y);
	glClearColor(_background.x, _background.y, _background.z, 1.f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_CCW);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glfwSwapInterval(0);
	glfwSwapBuffers(_wnd);

#if defined(__APPLE__)
	/* Poll for events once before starting a potentially
	   lengthy loading process. This is needed to be
	   classified as "interactive" by other software such
	   as iTerm2 */

	glfwPollEvents();
#endif

	setup_events();
	initialize();
}

Screen::~Screen()
{
	Core::instance().remove_screen(this);
	for (int i = 0; i < static_cast<int>(Cursor::Count); ++i)
		if (_cursors[i])
			glfwDestroyCursor(_cursors[i]);

	_painter = nullptr;

	if (_wnd)
	{
		glfwDestroyWindow(_wnd);
		_wnd = nullptr;
	}
}

void Screen::setup_events()
{
	glfwSetCursorPosCallback(_wnd, [](GLFWwindow* w, double x, double y)
	{
		Screen* s = static_cast<Screen*>(glfwGetWindowUserPointer(w));
		if (!s->_process_events) return;
		s->cursor_pos_callback(x, y);
	});

	glfwSetMouseButtonCallback(_wnd, [](GLFWwindow* w, int button, int action, int modifiers)
	{
		Screen* s = static_cast<Screen*>(glfwGetWindowUserPointer(w));
		if (!s->_process_events) return;
		s->mouse_button_callback(static_cast<Mouse>(button), static_cast<Action>(action), modifiers);
	});

	glfwSetKeyCallback(_wnd, [](GLFWwindow* w, int key, int scancode, int action, int mods)
	{
		Screen* s = static_cast<Screen*>(glfwGetWindowUserPointer(w));
		if (!s->_process_events) return;
		s->key_callback(static_cast<Key>(key), scancode, static_cast<Action>(action), mods);
	});

	glfwSetCharCallback(_wnd, [](GLFWwindow* w, unsigned int codepoint)
	{
		Screen* s = static_cast<Screen*>(glfwGetWindowUserPointer(w));
		if (!s->_process_events) return;
		s->char_callback(codepoint);
	});

	glfwSetDropCallback(_wnd, [](GLFWwindow* w, int count, char const** filenames)
	{
		Screen* s = static_cast<Screen*>(glfwGetWindowUserPointer(w));
		if (!s->_process_events) return;
		s->drop_callback(count, filenames);
	});

	glfwSetScrollCallback(_wnd, [](GLFWwindow* w, double x, double y)
	{
		Screen* s = static_cast<Screen*>(glfwGetWindowUserPointer(w));
		if (!s->_process_events) return;
		s->scroll_callback(x, y);
	});

	/* React to framebuffer size events -- includes window
	   size events and also catches things like dragging
	   a window from a Retina-capable screen to a normal
	   screen on Mac OS X */
	glfwSetFramebufferSizeCallback(_wnd, [](GLFWwindow* w, int width, int height)
	{
		Screen* s = static_cast<Screen*>(glfwGetWindowUserPointer(w));
		if (!s->_process_events) return;
		s->resize_callback(width, height);
	});
}

void Screen::initialize()
{
	Core::instance().add_screen(this);
	_painter = new Painter;

	glfwGetWindowSize(_wnd, &_size.x, &_size.y);
	glfwGetFramebufferSize(_wnd, &_fb_size.x, &_fb_size.y);
	_visible = glfwGetWindowAttrib(_wnd, GLFW_VISIBLE) != 0;
	_theme = new Theme(_painter);
	_mouse_pos = ivec2();
	_mouse_state = _modifiers = 0;
	_drag_active = false;
	_last_interaction = glfwGetTime();
	_process_events = true;
	_background = vec3(0.3f, 0.3f, 0.32f);

	for (int i = 0; i < static_cast<int>(Cursor::Count); ++i)
		_cursors[i] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR + i);
}

void Screen::caption(std::string const& caption)
{
	glfwSetWindowTitle(_wnd, caption.c_str());
	_caption = caption;
}

bool Screen::should_close() const
{
	return glfwWindowShouldClose(_wnd) == 1;
}

void Screen::should_close(bool value)
{
	glfwSetWindowShouldClose(_wnd, value ? 1 : 0);
}

void Screen::visible(bool value)
{
	if (value != _visible)
	{
		Widget::visible(value);
		if (_visible)
			glfwShowWindow(_wnd);
		else
			glfwHideWindow(_wnd);
	}
}

void Screen::size(ivec2 const& size)
{
	Widget::size(size);
	glfwSetWindowSize(_wnd, size.x, _size.y);
}

void Screen::width(ivec2::value_type x)
{
	size(ivec2(x, _size.y));
}

void Screen::height(ivec2::value_type y)
{
	size(ivec2(_size.x, y));
}

void Screen::draw_all()
{
	glClearColor(_background.x, _background.y, _background.z, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	draw_scene();
	draw_widgets();

	glfwSwapBuffers(_wnd);
}

void Screen::draw_widgets()
{
	if (!_visible) return;

	glfwMakeContextCurrent(_wnd);
	glfwGetFramebufferSize(_wnd, &_fb_size.x, &_fb_size.y);
	glfwGetWindowSize(_wnd, &_size.x, &_size.y);
	glViewport(0, 0, _fb_size.x, _fb_size.y);

	_pixel_ratio = static_cast<float>(_fb_size.x) / static_cast<float>(_size.x);
	_painter->begin_frame(_fb_size, _pixel_ratio);

	draw(_painter);

	float elapsed = static_cast<float>(glfwGetTime() - _last_interaction);
	if (elapsed > 0.5f)
	{
		Widget const* widget = find_widget(_mouse_pos);
		if (widget && !widget->tooltip().empty())
		{
			int tooltip_width = 150;

			_painter->font_face("sans");
			_painter->font_size(15.f);
			_painter->text_align(Align::Center | Align::Top);
			_painter->text_line_height(1.1f);

			ivec2 pos = widget->absolute_position() + ivec2(widget->width() / 2, widget->height() + 10);

			vec2 min, max;
			_painter->text_box_bounds(vec2(pos.x, pos.y), tooltip_width, widget->tooltip(), min, max);
			_painter->global_alpha(std::min(1.f, 2 * (elapsed - 0.5f)) * 0.8f);

			_painter->begin_path();
			_painter->fill_color(Color(0, 255));

			int h = static_cast<int>((max.x - min.x) / 2.f);
			_painter->rounded_rect(vec2(min.x - 4 - h, min.y - 4), vec2(static_cast<int>(max.x - min.x) + 8, static_cast<int>(max.y - min.y) + 8), 3);

			int px = static_cast<int>((max.x + min.x) / 2) - h;
			_painter->move_to(vec2(px, min.y - 10));
			_painter->line_to(vec2(px + 7, min.y + 1));
			_painter->line_to(vec2(px - 7, min.y + 1));
			_painter->fill();

			_painter->fill_color(Color(255, 255));
			_painter->font_blur(0.f);
			_painter->text_box(vec2(pos.x - h, pos.y), tooltip_width, widget->tooltip());
		}
	}

	_painter->end_frame();
}

bool Screen::keyboard_event(Key code, int scancode, Action action, int mods)
{
	if (_focus_path.size() > 0)
		for (auto it = _focus_path.rbegin() + 1; it != _focus_path.rend(); ++it)
			if ((*it)->focused() && (*it)->keyboard_event(code, scancode, action, mods))
				return true;

	return false;
}

bool Screen::keyboard_character_event(uint32 codepoint)
{
	if (_focus_path.size() > 0)
		for (auto it = _focus_path.rbegin() + 1; it != _focus_path.rend(); ++it)
			if ((*it)->focused() && (*it)->keyboard_character_event(codepoint))
				return true;

	return false;
}

bool Screen::drop_event(std::vector<std::string> const&)
{ return false; }

bool Screen::resize_event(ivec2 const&)
{ return false; }

bool Screen::cursor_pos_callback(double x, double y)
{
	ivec2 p(static_cast<int>(x) - 1, static_cast<int>(y) - 2);
	bool ret = false;
	_last_interaction = glfwGetTime();

	if (_drag_active)
	{
		Widget const* widget = find_widget(p);
		if (widget && widget->cursor() != _cursor)
		{
			_cursor = widget->cursor();
			glfwSetCursor(_wnd, _cursors[static_cast<int>(_cursor)]);
		}
	}
	else
	{
		if(_drag_widget == nullptr)
			ret = false;
		else
			ret = _drag_widget->mouse_drag_event(p - _drag_widget->parent()->absolute_position(), p - _mouse_pos, static_cast<Mouse>(_mouse_state), _modifiers);
	}

	if (!ret) ret = mouse_motion_event(p, p - _mouse_pos, static_cast<Mouse>(_mouse_state), _modifiers);
	_mouse_pos = p;

	return ret;
}

bool Screen::mouse_button_callback(Mouse button, Action action, int mods)
{
	_modifiers = mods;
	_last_interaction = glfwGetTime();

	if (_focus_path.size() > 1)
	{
		/*TODO: Implement when Window Widget is done
		Window const* window = dynamic_cast<Window*>(_focus_path[_focus_path.size() - 2]);
		if(window && window->modal())
			if(!window->contains(_mouse_pos))
				return false;*/
	}

	if (action == Action::Press)
		_mouse_state |= 1 << static_cast<int>(button);
	else
		_mouse_state &= ~(1 << static_cast<int>(button));

	auto drop_widget = find_widget(_mouse_pos);
	if (_drag_active && action == Action::Release && drop_widget != _drag_widget)
		_drag_widget->mouse_button_event(_mouse_pos - _drag_widget->parent()->absolute_position(), button, false, _modifiers);

	if (drop_widget && drop_widget->cursor() != _cursor)
	{
		_cursor = drop_widget->cursor();
		glfwSetCursor(_wnd, _cursors[static_cast<int>(_cursor)]);
	}

	if (action == Action::Press && button == Mouse::Left)
	{
		_drag_widget = find_widget(_mouse_pos);
		if (_drag_widget == this) _drag_widget = nullptr;
		_drag_active = _drag_widget != nullptr;
		if (!_drag_active) update_focus(nullptr);
	}
	else
	{
		_drag_active = false;
		_drag_widget = nullptr;
	}

	return mouse_button_event(_mouse_pos, button, action == Action::Press, _modifiers);
}

bool Screen::key_callback(Key code, int scancode, Action action, int mods)
{
	_last_interaction = glfwGetTime();
	return keyboard_event(code, scancode, action, mods);
}

bool Screen::char_callback(uint32 codepoint)
{
	_last_interaction = glfwGetTime();
	return keyboard_character_event(codepoint);
}

bool Screen::drop_callback(int count, char const** filenames)
{
	std::vector<std::string> arg(static_cast<std::size_t>(count));
	for (int i = 0; i < count; ++i)
		arg[i] = filenames[i];
	return drop_event(arg);
}

bool Screen::scroll_callback(double x, double y)
{
	_last_interaction = glfwGetTime();
	if (_focus_path.size() > 1)
	{
		/*TODO: Implement when Window Widget is done
		Window const* window = dynamic_cast<Window*>(_focus_path[_focus_path.size() - 2]);
		if(window && window->modal())
			if(!window->contains(_mouse_pos))
				return false;*/
	}
	return scroll_event(_mouse_pos, ivec2(static_cast<int>(x), static_cast<int>(y)));
}

bool Screen::resize_callback(int, int)
{
	ivec2 fb_size, size;
	glfwGetFramebufferSize(_wnd, &fb_size.x, &fb_size.y);
	glfwGetWindowSize(_wnd, &size.x, &size.y);

	if (fb_size == ivec2(0, 0) || size == ivec2(0, 0))
		return false;

	_fb_size = fb_size;
	_size = size;
	_last_interaction = glfwGetTime();

	return resize_event(_size);
}

void Screen::update_focus(Widget* widget)
{
	for (auto w : _focus_path)
	{
		if (!w->focused())
			continue;
		w->focus_event(false);
	}
	_focus_path.clear();
	//Widget* window = nullptr;
	while (widget)
	{
		_focus_path.push_back(widget);
		// TODO: Implement when Window Widget is done
		// if (dynamic_cast<Window*>(widget))
		//	window = widget;
		widget = widget->parent();
	}
	for (auto it = _focus_path.rbegin(); it != _focus_path.rend(); ++it)
		(*it)->focus_event(true);

	// TODO: Implement when Window Widget is done
	// if (window)
	//	moveWindowToFront((Window *) window);
}

// TODO: Implement when Window Widget is done
// void Screen::dispose_window(Window* window) { }

// TODO: Implement when Window Widget is done
// void Screen::center_window(WIndow* window) { }

// TODO: Implement when Window Widget is done
// void Screen::move_window_to_front(Window* window) { }
