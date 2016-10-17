#pragma once

#include <spdlog/spdlog.h>
#include <bgfx/bgfx.h>
#include <memory>
#include <string>
#include <ABI.h>
#include <list>

#include "InputEnums.hpp"
#include "Math.hpp"

#ifndef GOBLIN_DISABLE_LOG_MACROS
#define TRACE(...) Engine::ptr()->log()->trace(__VA_ARGS__)
#define DEBUG(...) Engine::ptr()->log()->debug(__VA_ARGS__)
#define INFO(...) Engine::ptr()->log()->info(__VA_ARGS__)
#define WARN(...) Engine::ptr()->log()->warn(__VA_ARGS__)
#define ERR(...) Engine::ptr()->log()->error(__VA_ARGS__)
#define FATAL(...) Engine::ptr()->log()->critical(__VA_ARGS__)
#else
#define TRACE(...)
#define DEBUG(...)
#define INFO(...)
#define WARN(...)
#define ERR(...)
#define FATAL(...)
#endif

class E_ABI(engine) Engine final
{
	static std::unique_ptr<Engine> _inst;

	std::shared_ptr<spdlog::logger> _log;
	std::list<std::string> _args;
	i32 _exit_code;
	boolean _running;

	struct SDL_Window* _wnd;

	i32 run(class Application* app);

public:
	template<class AppType>
	static int run(int argc, char* argv[]);

	static inline Engine* ptr()
	{ return _inst.get(); }

	static inline Engine& ref()
	{ return *ptr(); }

	Engine(int argc, char** argv);

	virtual ~Engine();

	Engine(Engine const& other) = delete;

	Engine(Engine&& other) = default;

	Engine& operator=(Engine const& other) = delete;

	Engine& operator=(Engine&& other) = default;

	inline std::list<std::string> arguments() const
	{ return _args; }

	inline std::shared_ptr<spdlog::logger> log() const
	{ return _log; }

	glm::tvec2<u16> window_size() const;

	inline void exit(i32 code = {0})
	{
		_running = false;
		_exit_code = code;
	}
};

class E_ABI(engine) Application
{
public:
	virtual ~Application()
	{ }

	Application(Application const&) = delete;

	Application(Application&&) = default;

	Application& operator=(Application const&) = delete;

	Application& operator=(Application&&) = default;

	static Engine& engine()
	{ return Engine::ref(); }

	virtual void initialize() = 0;

	virtual void update() = 0;

	virtual void mouse_button_event(ivec2 const&, MouseButton const&, boolean const&)
	{ }

	virtual void mouse_move_event(ivec2 const&)
	{ }

	virtual void scroll_event(ivec2 const&)
	{ }

	virtual void keyboard_event(Key const&, boolean const&)
	{ }

	virtual void keyboard_character_event(char const*)
	{ }

	virtual void resize_event(u16 const&, u16 const&)
	{ }

protected:
	Application()
	{ }
};

template<class AppType>
inline int Engine::run(int argc, char* argv[])
{
	static_assert(std::is_base_of<Application, AppType>::value, "AppType must be a subclass of Application");
	static_assert(std::is_constructible<AppType>::value, "AppType must have an empty constructor");

	_inst = std::make_unique<Engine>(argc, argv);
	return (int) _inst->run(new AppType);
}
