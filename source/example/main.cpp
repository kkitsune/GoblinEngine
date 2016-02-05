#include <engine/Core.hpp>
#include <iostream>

#include <engine/Screen.hpp>

using namespace std;

int main(int /*argc*/, char* /*argv*/[])
{
	Core::create();

	try
	{
		Screen wnd;
	}
	catch(exception& e)
	{
		cerr << "Exception caught : " << e.what() << endl;
	}

	Core::destroy();
	return 0;
}
