#include <engine/Engine.hpp>

class TestGame : public Application
{
	u16 _w, _h;

public:
	TestGame() : _w{0_u16}, _h{0_u16}
	{
		WARN("Goblin Engine Example => Constructed");
	}

	virtual ~TestGame()
	{
		WARN("Goblin Engine Example => Destroyed");
	}

	virtual void initialize() override
	{
		auto size = engine().window_size();
		_w = size.x;
		_h = size.y;

		bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);
		bgfx::setDebug(BGFX_DEBUG_TEXT);

		WARN("Goblin Engine Example => Initialized");
	}

	virtual void update() override
	{
		bgfx::setViewRect(0, 0, 0, (uint16_t) _w, (uint16_t) _h);
		bgfx::touch(0);

		bgfx::dbgTextClear();
		bgfx::dbgTextPrintf(2, 1, 0x0f, "Goblin Engine Example => Working as expected!");

		bgfx::frame();
	}

	virtual void keyboard_event(Key const& key, boolean const& down) override
	{
		if (key == Key::Escape && down)
			engine().exit();
	}

	virtual void resize_event(u16 const& w, u16 const& h) override
	{
		_w = w;
		_h = h;
	}
};

int main(int argc, char* argv[])
{ return Engine::run<TestGame>(argc, argv); }
