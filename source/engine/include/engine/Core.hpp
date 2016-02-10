#pragma once

#include <memory>

#include "Common.hpp"

#include "Utils/ThreadPool.hpp"
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

	void reset_window_hints();

	void poll_events();

	template<typename Func, typename... Args>
	inline auto submit_task(Func f, Args&&... args)
	{
		return _pool.submit(f, args...);
	}

private:
	static Core* _inst;

	std::unique_ptr<Settings> _settings;
	ThreadPool _pool;
};
