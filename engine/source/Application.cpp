#include "engine/Application.hpp"

Application::Application(Engine* e) : _engine(e)
{
}

Application::~Application()
{
}

void Application::resize_event(int width, int height)
{
	glViewport(0, 0, width, height);
}
