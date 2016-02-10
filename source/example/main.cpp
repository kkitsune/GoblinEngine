#include <engine/Core.hpp>
#include <iostream>

#include <engine/Screen.hpp>

using namespace std;

class ExampleScreen : public Screen
{
public:
	ExampleScreen() : Screen(ivec2(960, 640), "GoblinEngine Example")
	{ }

	virtual ~ExampleScreen()
	{ }


	virtual bool keyboard_event(Key code, int, Action action, int) override
	{
		if(code == Key::Escape && action == Action::Press)
		{
			should_close(true);
			return true;
		}

		return false;
	}
};

int main(int /*argc*/, char* /*argv*/[])
{
	Core::create();

	try
	{
		Ref<ExampleScreen> ex(new ExampleScreen);
		ex->visible(true);
		Core::instance().run();
	}
	catch(exception& e)
	{
		cerr << "Exception caught : " << e.what() << endl;
	}

	Core::destroy();
	return 0;
}
