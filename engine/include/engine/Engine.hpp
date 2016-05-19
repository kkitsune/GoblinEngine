#pragma once

#include <engine/config.h>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <type_traits>
#include <string>
#include <list>

class Application;

class ENGINE_API Engine final
{
public:
	Engine(int argc, char** argv);

	virtual ~Engine();

	Engine(Engine const& other) = delete;

	Engine(Engine&& other) = default;

	Engine& operator=(Engine const& other) = delete;

	Engine& operator=(Engine&& other) = default;

	inline std::list<std::string> arguments() const
	{ return _args; }

	glm::ivec2 framebuffer_size() const;

	glm::ivec2 window_size() const;

	inline float pixel_ratio() const
	{ return (float) framebuffer_size().x / (float) window_size().x; }

	int run(Application* app);

	template<class AppType>
	int run();

	inline void exit(int code = 0)
	{
		_running = false;
		_exit_code = code;
	}

private:
	void setup_events();

	std::list<std::string> _args;
	int _exit_code;
	bool _running;

	struct GLFWwindow* _wnd;
};

#include "Application.hpp"

template<class AppType>
inline int Engine::run()
{
	static_assert(std::is_base_of<Application, AppType>::value, "AppType must be a subclass of Application");
	static_assert(std::is_constructible<AppType, Engine*>::value, "AppType must have a constructor taking an Engine pointer");

	return run(new AppType(this));
}
