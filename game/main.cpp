#include <engine/Application.hpp>
#include <engine/utils/imgui.h>
#include <iostream>

using namespace glm;
using namespace std;

class TestGame : public Application
{
	float _main_size;
	bool _show_metrics, _show_test;

public:
	TestGame(Engine* e) : Application(e), _show_metrics(false), _show_test(false)
	{
		cerr << "TestGame => Constructed" << endl;
		for(auto i : engine().arguments())
			cout << " - " << i << endl;
		cout << endl;
	}

	virtual ~TestGame()
	{
		cerr << "TestGame => Destroyed" << endl;
	}

	virtual void initialize() override
	{
		cerr << "TestGame => Initialized" << endl;
		_main_size = (float) engine().framebuffer_size().x / 4.f;
		if(_main_size < 350.f) _main_size = 350.f;
	}

	virtual void update(Duration) override
	{ }

	virtual void frame_start() override
	{ }

	virtual void frame_end() override
	{
		if(_show_test) ImGui::ShowTestWindow(nullptr);
		if(_show_metrics) ImGui::ShowMetricsWindow(nullptr);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
		ImGui::SetNextWindowPos(ImVec2(engine().framebuffer_size().x - _main_size, 0), ImGuiSetCond_Always);
		ImGui::SetNextWindowSize(ImVec2(_main_size, engine().framebuffer_size().y));
		ImGui::Begin("MainWindow", nullptr,
		             ImGuiWindowFlags_NoCollapse |
		             ImGuiWindowFlags_NoTitleBar |
		             ImGuiWindowFlags_NoMove |
		             ImGuiWindowFlags_NoResize |
					 ImGuiWindowFlags_NoFocusOnAppearing |
					 ImGuiWindowFlags_NoBringToFrontOnFocus |
					 ImGuiWindowFlags_NoSavedSettings);
		ImGui::PopStyleVar();

		ImGui::BeginChild("buttons");
		if(ImGui::Button("Exit")) engine().exit();
		ImGui::SameLine();
		ImGui::Checkbox("Metrics", &_show_metrics);
		ImGui::SameLine();
		ImGui::Checkbox("Test", &_show_test);
		ImGui::EndChild();
		ImGui::Separator();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
		ImGui::End();
		ImGui::PopStyleVar();
	}

	virtual void keyboard_event(Key key, int, Action action, int) override
	{
		if(key == Key::Escape && action == Action::Press)
		{
			cerr << "TestGame => Escape pressed" << endl;
			engine().exit();
		}
	}

	virtual void resize_event(int width, int height) override
	{
		Application::resize_event(width, height);
		_main_size = (float) width / 4.f;
		if(_main_size < 350.f) _main_size = 350.f;
	}
};

int main(int argc, char* argv[])
{ return Engine(argc, argv).run<TestGame>(); }
