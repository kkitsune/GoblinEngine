#pragma once

#include <boost/filesystem/path.hpp>
#include <engine/engine_api.h>
#include <glm/glm.hpp>

class ENGINE_API Settings
{
	friend class Core;

public:
	~Settings();

	Settings(Settings const& other) = delete;

	Settings(Settings&& other) = delete;

	Settings& operator=(Settings const& other) = delete;

	Settings& operator=(Settings&& other) = delete;

	void load();

	void save() const;

	boost::filesystem::path texture_path() const
	{ return _texture; }

	void texture_path(boost::filesystem::path const& path)
	{ _texture = path; }

	boost::filesystem::path shader_path() const
	{ return _shader; }

	void shader_path(boost::filesystem::path const& path)
	{ _shader = path; }

	boost::filesystem::path font_path() const
	{ return _font; }

	void font_path(boost::filesystem::path const& path)
	{ _font = path; }

	boost::filesystem::path script_path() const
	{ return _script; }

	void script_path(boost::filesystem::path const& path)
	{ _script = path; }

	bool gamepad_enabled() const
	{ return _gp_enabled; }

	void gamepad_enabled(bool value);

private:
	Settings();

	boost::filesystem::path _texture, _shader, _font, _script;
	bool _gp_enabled;
};
