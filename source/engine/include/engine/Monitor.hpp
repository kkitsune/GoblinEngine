#pragma once

#include <list>

#include "DisplayMode.hpp"

class ENGINE_API Monitor
{
	friend class Screen;

public:
	static const std::list<Monitor> all();

	static const Monitor primary();

	Monitor(Monitor const& other);

	Monitor(Monitor&& other);

	Monitor& operator=(Monitor const& other);

	Monitor& operator=(Monitor&& other);

	ivec2 position() const;

	ivec2 physical_size() const;

	std::string name() const;

	const std::list<DisplayMode> available_modes() const;

	const DisplayMode display_mode() const;

private:
	Monitor(struct GLFWmonitor* monitor);

	struct GLFWmonitor* _monitor;
};
