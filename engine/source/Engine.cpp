#include <engine/Engine.hpp>

#include <bgfx/bgfxplatform.h>
#include <SDL2/SDL.h>
#include <bx/debug.h>

BX_PRAGMA_DIAGNOSTIC_PUSH()
BX_PRAGMA_DIAGNOSTIC_IGNORED_CLANG("-Wextern-c-compat")

#include <SDL2/SDL_syswm.h>

BX_PRAGMA_DIAGNOSTIC_POP()

#if defined(None) // X11 defines this...
#	undef None
#endif // defined(None)

inline bool sdlSetWindow(SDL_Window* _window)
{
	SDL_SysWMinfo wmi;
	SDL_VERSION(&wmi.version);
	if (!SDL_GetWindowWMInfo(_window, &wmi))
	{
		return false;
	}

	bgfx::PlatformData pd;
#	if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
	pd.ndt = wmi.info.x11.display;
	pd.nwh = (void*) (uintptr_t) wmi.info.x11.window;
#	elif BX_PLATFORM_OSX
	pd.ndt          = NULL;
		pd.nwh          = wmi.info.cocoa.window;
#	elif BX_PLATFORM_WINDOWS
		pd.ndt          = NULL;
		pd.nwh          = wmi.info.win.window;
#	elif BX_PLATFORM_STEAMLINK
		pd.ndt          = wmi.info.vivante.display;
		pd.nwh          = wmi.info.vivante.window;
#	endif // BX_PLATFORM_
	pd.context = NULL;
	pd.backBuffer = NULL;
	pd.backBufferDS = NULL;
	bgfx::setPlatformData(pd);

	return true;
}

std::unique_ptr<Engine> Engine::_inst{};

struct EngineCallback : public bgfx::CallbackI
{
	std::shared_ptr<spdlog::logger> _log;

	EngineCallback()
	{
		_log = std::make_shared<spdlog::logger>("bgfx", std::make_shared<spdlog::sinks::simple_file_sink_st>("bgfx.log", true));
		_log->set_level(spdlog::level::trace);
		spdlog::register_logger(_log);
	}

	virtual ~EngineCallback() override
	{}

	virtual void fatal(bgfx::Fatal::Enum _code, char const* _str) override
	{
		if (bgfx::Fatal::DebugCheck == _code)
			bx::debugBreak();
		else
		{
			_log->error("0x{}: {}", _code, _str);
			std::abort();
		}
	}

	virtual void traceVargs(char const* _filePath, uint16_t _line, char const* _format, va_list _argList) override
	{
		char temp[2048];
		char* out = temp;
		va_list argListCopy;
		va_copy(argListCopy, _argList);
		int32_t len = snprintf(out, sizeof(temp), "%s (%d): ", _filePath, _line);
		int32_t total = len + vsnprintf(out + len, sizeof(temp) - len, _format, argListCopy);
		va_end(argListCopy);
		if ((int32_t) sizeof(temp) < total)
		{
			out = (char*) alloca(total + 1);
			memcpy(out, temp, (size_t) len);
			vsnprintf(out + len, (size_t) (total - len), _format, _argList);
		}
		out[total - 1] = '\0';
		_log->trace(out);
	}

	virtual uint32_t cacheReadSize(uint64_t) override
	{ return 0; }

	virtual bool cacheRead(uint64_t, void*, uint32_t) override
	{ return false; }

	virtual void cacheWrite(uint64_t, void const*, uint32_t) override
	{}

	virtual void screenShot(char const*, uint32_t, uint32_t, uint32_t, const void*, uint32_t, bool) override
	{}

	virtual void captureBegin(uint32_t, uint32_t, uint32_t, bgfx::TextureFormat::Enum, bool) override
	{}

	virtual void captureEnd() override
	{}

	virtual void captureFrame(void const*, uint32_t) override
	{}
};

Engine::Engine(int argc, char** argv) : _exit_code{0}, _running{false}
{
	for (int i = 0; i < argc; ++i)
		_args.push_back({argv[i]});

	std::vector<spdlog::sink_ptr> sinks
			{
					std::make_shared<spdlog::sinks::ansicolor_sink>(std::make_shared<spdlog::sinks::stdout_sink_st>()),
					std::make_shared<spdlog::sinks::simple_file_sink_st>("GoblinEngine.log", true)
			};
	_log = std::make_shared<spdlog::logger>("engine", begin(sinks), end(sinks));
	_log->set_level(spdlog::level::trace);
	spdlog::register_logger(_log);

	std::string cmd;
	for (auto a : _args)
		cmd += a + " ";
	_log->warn("Invocation : {}", cmd);
}

Engine::~Engine()
{}

i32 Engine::run(Application* app)
{
	TRACE("Engine::run => Begin");
	_running = {true};
	_exit_code = {0};

	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_DisplayMode ask_mode;
	ask_mode.format = SDL_PIXELFORMAT_RGBA8888;
	ask_mode.w = 1920;
	ask_mode.h = 1080;
	ask_mode.refresh_rate = 60;
	SDL_DisplayMode mode;
	SDL_GetClosestDisplayMode(0, &ask_mode, &mode);

	DEBUG("Engine::run => Display Mode selected : {}x{}@{}", mode.w, mode.h, mode.refresh_rate);
	_wnd = SDL_CreateWindow("BGFX Rendering Window",
	                        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mode.w, mode.h,
	                        SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN);
	SDL_SetWindowDisplayMode(_wnd, &mode);
	TRACE("Engine::run => Window created");

	sdlSetWindow(_wnd);
	bgfx::renderFrame();

	TRACE("Engine::run => Initializing application");
	EngineCallback cb;
	bgfx::init(bgfx::RendererType::Count, BGFX_PCI_ID_NONE, 0, &cb);
	bgfx::reset((uint32_t) mode.w, (uint32_t) mode.h, BGFX_RESET_FULLSCREEN | BGFX_RESET_VSYNC);

	app->initialize();
	bgfx::frame();
	TRACE("Engine::run => Application initialized");

	TRACE("Engine::run => Entering main loop");
	while (_running)
	{
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_WINDOWEVENT_CLOSE)
				_running = false;
			if (e.type == SDL_KEYUP || e.type == SDL_KEYDOWN)
				app->keyboard_event(static_cast<Key>(e.key.keysym.scancode), e.type == SDL_KEYDOWN);
			if (e.type == SDL_TEXTINPUT)
				app->keyboard_character_event(e.text.text);
			if (e.type == SDL_MOUSEMOTION)
				app->mouse_move_event({e.motion.xrel, e.motion.yrel});
			if (e.type == SDL_MOUSEBUTTONUP || e.type == SDL_MOUSEBUTTONDOWN)
				app->mouse_button_event({e.button.x, e.button.y}, static_cast<MouseButton>(e.button.button),
				                        e.type == SDL_MOUSEBUTTONDOWN);
			if (e.type == SDL_MOUSEWHEEL)
				app->scroll_event({e.wheel.x, e.wheel.y});
			if (e.type == SDL_WINDOWEVENT_RESIZED)
				app->resize_event(narrow_cast<u16>(make_unsigned(i32{e.window.data1})),
				                  narrow_cast<u16>(make_unsigned(i32{e.window.data2})));
		}

		app->update();
	}

	delete app;

	SDL_DestroyWindow(_wnd);
	_wnd = nullptr;
	TRACE("Engine::run => Window destroyed");

	bgfx::shutdown();
	SDL_Quit();
	TRACE("Engine::run => Bye ({})\n", (int) _exit_code);
	return _exit_code;
}

glm::tvec2<u16> Engine::window_size() const
{
	int x, y;
	SDL_GetWindowSize(_wnd, &x, &y);
	return {narrow_cast<u16>(make_unsigned(i32{x})), narrow_cast<u16>(make_unsigned(i32{y}))};
}
