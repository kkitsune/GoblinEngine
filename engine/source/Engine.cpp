#include "engine/Engine.hpp"

#include <engine/utils/imgui.h>
#include <GLFW/glfw3.h>

#include "deps/imgui/imgui_impl_glfw_gl3.h"

using namespace std;
using namespace glm;

Engine::Engine(int argc, char** argv) : _wnd(nullptr)
{
	for(int i = 0; i < argc; ++i)
		_args.push_back(string{argv[i]});
}

Engine::~Engine()
{
}

int Engine::run(Application* app)
{
	_running = true;
	_exit_code = 0;
	glfwInit();

	glfwDefaultWindowHints();
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	_wnd = glfwCreateWindow(960, 640, "GoblinEngine Rendering Window", nullptr, nullptr);
	glfwMakeContextCurrent(_wnd);
	gladLoadGL();
	glfwSetWindowUserPointer(_wnd, app);

	ImGui_ImplGlfwGL3_Init(_wnd, false);

	auto fb_size = framebuffer_size();
	glViewport(0, 0, fb_size.x, fb_size.y);
	glClearColor(0.2f, 0.2f, 0.2f, 1.f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glfwSwapInterval(0);
	glfwSwapBuffers(_wnd);

#if defined(__APPLE__)
	/* Poll for events once before starting a potentially
	   lengthy loading process. This is needed to be
	   classified as "interactive" by other software such
	   as iTerm2 */

	glfwPollEvents();
#endif

	setup_events();
	app->initialize();

	Seconds accumulator = 0.f;
	while(_running)
	{
		_running = glfwWindowShouldClose(_wnd) == 0;
		accumulator += ImGui::GetIO().DeltaTime;

		glfwPollEvents();
		ImGui_ImplGlfwGL3_NewFrame();

		if(accumulator >= app->fixed_time_step())
		{
			app->update(accumulator);
			accumulator = 0.f;
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		app->frame_start();
		app->frame();
		app->frame_end();

		ImGui::Render();
		glfwSwapBuffers(_wnd);
	}

	ImGui_ImplGlfwGL3_Shutdown();

	glfwDestroyWindow(_wnd);
	_wnd = nullptr;

	glfwTerminate();
	delete app;
	return _exit_code;
}

glm::ivec2 Engine::framebuffer_size() const
{
	ivec2 ret;
	glfwGetFramebufferSize(_wnd, &ret.x, &ret.y);
	return ret;
}

glm::ivec2 Engine::window_size() const
{
	ivec2 ret;
	glfwGetWindowSize(_wnd, &ret.x, &ret.y);
	return ret;
}

void Engine::setup_events()
{
	glfwSetCursorPosCallback(_wnd, [](GLFWwindow* w, double x, double y)
	{
		if (!ImGui::GetIO().WantCaptureMouse)
		{
			Application* a = static_cast<Application*>(glfwGetWindowUserPointer(w));
			a->mouse_move_event(dvec2(x, y));
		}
	});

	glfwSetMouseButtonCallback(_wnd, [](GLFWwindow* w, int button, int action, int mods)
	{
		ImGui_ImplGlfwGL3_MouseButtonCallback(w, button, action, mods);
		if (!ImGui::GetIO().WantCaptureMouse)
		{
			Application* a = static_cast<Application*>(glfwGetWindowUserPointer(w));
			a->mouse_button_event(static_cast<MouseButton>(button), action == GLFW_PRESS, mods);
		}
	});

	glfwSetKeyCallback(_wnd, [](GLFWwindow* w, int key, int scancode, int action, int mods)
	{
		ImGui_ImplGlfwGL3_KeyCallback(w, key, scancode, action, mods);
		if(!ImGui::GetIO().WantCaptureKeyboard)
		{
			Application* a = static_cast<Application*>(glfwGetWindowUserPointer(w));
			a->keyboard_event(static_cast<Key>(key), scancode, static_cast<Action>(action), mods);
		}
	});

	glfwSetCharCallback(_wnd, [](GLFWwindow* w, unsigned int codepoint)
	{
		ImGui_ImplGlfwGL3_CharCallback(w, codepoint);
		if(!ImGui::GetIO().WantTextInput)
		{
			Application* a = static_cast<Application*>(glfwGetWindowUserPointer(w));
			a->keyboard_character_event(codepoint);
		}
	});

	/*glfwSetDropCallback(_wnd, [](GLFWwindow* w, int count, char const** filenames)
	{
		Application* a = static_cast<Application*>(glfwGetWindowUserPointer(w));
	});*/

	glfwSetScrollCallback(_wnd, [](GLFWwindow* w, double x, double y)
	{
		ImGui_ImplGlfwGL3_ScrollCallback(w, x, y);
		if (!ImGui::GetIO().WantCaptureMouse)
		{
			Application* a = static_cast<Application*>(glfwGetWindowUserPointer(w));
			a->scroll_event(dvec2(x, y));
		}
	});

	/* React to framebuffer size events -- includes window
	   size events and also catches things like dragging
	   a window from a Retina-capable screen to a normal
	   screen on Mac OS X */
	glfwSetFramebufferSizeCallback(_wnd, [](GLFWwindow* w, int width, int height)
	{
		Application* a = static_cast<Application*>(glfwGetWindowUserPointer(w));
		a->resize_event(width, height);
	});
}
