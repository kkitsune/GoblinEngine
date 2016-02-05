#include <engine/Monitor.hpp>
#include <GLFW/glfw3.h>

const std::list<Monitor> Monitor::all()
{
	int count;
	struct GLFWmonitor** monitors = glfwGetMonitors(&count);
	std::list<Monitor> ret;
	for (int i = 0; i < count; i++)
		ret.push_back(Monitor(monitors[i]));
	return ret;
}

const Monitor Monitor::primary()
{
	return Monitor(glfwGetPrimaryMonitor());
}

Monitor::Monitor(GLFWmonitor* monitor) : _monitor(monitor)
{ }

Monitor::Monitor(Monitor const& other) : _monitor(other._monitor)
{ }

Monitor::Monitor(Monitor&& other) : _monitor(std::move(other._monitor))
{ }

Monitor& Monitor::operator=(Monitor const& other)
{
	_monitor = other._monitor;
	return *this;
}

Monitor& Monitor::operator=(Monitor&& other)
{
	_monitor = std::move(other._monitor);
	return *this;
}

glm::ivec2 Monitor::position() const
{
	glm::ivec2 ret;
	glfwGetMonitorPos(_monitor, &ret.x, &ret.y);
	return ret;
}

glm::ivec2 Monitor::physical_size() const
{
	glm::ivec2 ret;
	glfwGetMonitorPhysicalSize(_monitor, &ret.x, &ret.y);
	return ret;
}

std::string Monitor::name() const
{
	return glfwGetMonitorName(_monitor);
}

const std::list<DisplayMode> Monitor::available_modes() const
{
	int count;
	const GLFWvidmode* modes = glfwGetVideoModes(_monitor, &count);
	std::list<DisplayMode> ret;
	for (int i = 0; i < count; i++)
		ret.push_back(DisplayMode(&modes[i]));
	return ret;
}

const DisplayMode Monitor::display_mode() const
{
	return DisplayMode(glfwGetVideoMode(_monitor));
}
