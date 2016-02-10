#pragma once

#include <memory>
#include <list>

#include "Common.hpp"

#include "utils/ThreadPool.hpp"
#include "Settings.hpp"
#include "Object.hpp"

class ENGINE_API Core final
{
	friend class Screen;

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

	template<typename Func, typename... Args>
	inline auto submit_task(Func f, Args&&... args)
	{
		return _pool.submit(f, args...);
	}

	void run();

private:
	static Core* _inst;

	void add_screen(class Screen* scr);

	void remove_screen(class Screen* scr);

	std::list<Ref<class Screen>> _screens;

	std::unique_ptr<Settings> _settings;
	ThreadPool _pool;
};
