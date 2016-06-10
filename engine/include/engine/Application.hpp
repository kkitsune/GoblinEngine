#pragma once

#include <engine/InputEnums.hpp>
#include <engine/Engine.hpp>
#include <engine/Time.hpp>

class ENGINE_API Application
{
public:
	virtual ~Application();

	Application(Application const& other) = delete;

	Application(Application&& other) = default;

	Application& operator=(Application const& other) = delete;

	Application& operator=(Application&& other) = default;

	Engine& engine() const
	{ return *_engine; }

	Engine& engine()
	{ return *_engine; }

	virtual Seconds fixed_time_step() const
	{ return 1.f / 30.f; }

	virtual void initialize() = 0;

	virtual void update(Seconds delta) = 0;

	virtual void frame_start()
	{ }

	virtual void frame()
	{ }

	virtual void frame_end()
	{ }

	virtual void mouse_button_event(MouseButton, bool, int)
	{ }

	virtual void mouse_move_event(glm::dvec2 const&)
	{ }

	virtual void scroll_event(glm::dvec2 const&)
	{ }

	virtual void keyboard_event(Key, int, Action, int)
	{ }

	virtual void keyboard_character_event(unsigned int)
	{ }

	virtual void resize_event(int width, int height);

protected:
	Application(Engine* e);

	Engine* _engine;
};
