#include <Engine/Core.hpp>
#include <GLFW/glfw3.h>
#include <fstream>
#include <iomanip>

#include <Engine/Gamepad.hpp>

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

Core::Core() : _settings(new Settings)
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

void Core::poll_events()
{
	glfwPollEvents();

	if (_settings->gamepad_enabled())
		gamepad_update();
}
