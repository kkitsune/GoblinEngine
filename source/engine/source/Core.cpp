#include <engine/Core.hpp>
#include <GLFW/glfw3.h>
#include <fstream>
#include <iomanip>

#include <engine/Gamepad.hpp>
#include <engine/Screen.hpp>

Core* Core::_inst = nullptr;

void Core::create()
{
	if (_inst != nullptr)
		destroy();

	_inst = new Core;
}

void Core::destroy()
{
	if (_inst == nullptr) return;
	delete _inst;
	_inst = nullptr;
}

Core& Core::instance()
{
	if (_inst == nullptr) throw std::runtime_error("Engine is not started");
	return *_inst;
}

Core::Core() : _settings(new Settings), _pool()
{
	_settings->load();
	glfwInit();

	if (_settings->gamepad_enabled())
		gamepad_init();
}

Core::~Core()
{
	if (_settings->gamepad_enabled())
		gamepad_shutdown();

	glfwTerminate();
}

void Core::reset_window_hints()
{
	glfwDefaultWindowHints();
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
}

void Core::add_screen(Screen* scr)
{
	_screens.push_back(scr);
}

void Core::remove_screen(Screen* scr)
{
	_screens.remove(scr);
}

void Core::run()
{
	bool running = true;

	std::thread refresh_thread = std::thread(
	[&running]()
	{
		std::chrono::milliseconds time(50);
		while (running)
		{
			std::this_thread::sleep_for(time);
			glfwPostEmptyEvent();
		}
	});

	while(running)
	{
		if (_settings->gamepad_enabled())
			gamepad_update();

		int num_scr = 0;
		for(auto scr : _screens)
		{
			if(!scr->visible())
				continue;
			else if(scr->should_close())
			{
				scr->visible(false);
				continue;
			}

			scr->draw_all();
			++num_scr;
		}

		running = num_scr > 0;
		if(running)
			glfwWaitEvents();
	}
}
