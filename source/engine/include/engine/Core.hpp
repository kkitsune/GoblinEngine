#pragma once

#include <engine/engine_api.h>
#include <memory>

#include "Settings.hpp"

class ENGINE_API Core final
{
	Core();

public:
	static void create();

	static void destroy();

	static Core& instance();

	~Core();

	Core(Core const& other) = delete;

	Core(Core&& other) = delete;

	Core& operator=(Core const& other) = delete;

	Core& operator=(Core&& other) = delete;

	Settings& settings()
	{ return *(_settings.get()); }

	void poll_events();

private:
	static Core* _inst;

	std::unique_ptr<Settings> _settings;
};
