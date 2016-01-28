#include <Engine/Utils/Json.hpp>
#include <boost/filesystem.hpp>
#include <Engine/Core.hpp>
#include <stdexcept>
#include <fstream>

using namespace boost::filesystem;
using namespace std;

using Json = nlohmann::json;

Settings::Settings()
{
	_texture = "data/textures";
	_shader = "data/shaders";
	_font = "data/fonts";
	_script = "data/scripts";
#ifdef GOBLIN_NO_GAMEPAD
	_gp_enabled = false;
#else
	_gp_enabled = true;
#endif
}

Settings::~Settings()
{
}

void Settings::load()
{
	if (!exists("settings.js"))
	{
		save();
		return;
	}

	std::ifstream file("settings.js");
	string err;
	Json js = Json::parse(file);
	if(js.is_null()) throw runtime_error("There was an error while loading the settings : " + err);

	if(!js["paths"].is_object()) throw runtime_error("Settings' paths is not a JSON Object");
	_texture = js["paths"].value<string>("texture", "data/textures");
	_shader = js["paths"].value<string>("shader", "data/shaders");
	_font = js["paths"].value<string>("font", "data/fonts");
	_script = js["paths"].value<string>("script", "data/scripts");

	if(!js["gamepad"].is_boolean()) throw runtime_error("Settings' joystick is not a JSON Boolean");
	_gp_enabled = js.at("gamepad");
}

void Settings::save() const
{
	Json root = Json{
		{"paths", {
			{"texture", _texture.generic_string()},
			{"shader",  _shader.generic_string()},
			{"font",    _font.generic_string()},
			{"script",  _script.generic_string()}
		}},
		{"gamepad", _gp_enabled}
	};

	string js = root.dump(2);
	std::ofstream file("settings.js", ios::trunc);
	file.write(js.c_str(), js.size());
}

void Settings::gamepad_enabled(bool value)
{
#ifdef GOBLIN_NO_GAMEPAD
	_gp_enabled = false;
#else
	_gp_enabled = value;
#endif
}
