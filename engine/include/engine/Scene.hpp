#pragma once

#include <engine/utils/string_id/string_id.hpp>
#include <engine/utils/string_id/database.hpp>
#include <engine/Time.hpp>

#include <unordered_map>
#include <glm/glm.hpp>
#include <string>

class ENGINE_API Entity
{
	friend class Scene;

public:
	~Entity()
	{ }

private:
	Entity(foonathan::string_id::string_id const& name, class Scene* scene) : _name(name), _scene(scene)
	{ }

	foonathan::string_id::string_id _name;
	class Scene* _scene;
};

class ENGINE_API Scene
{
public:
	Scene();

	virtual ~Scene();

	Scene(Scene const& other) = delete;

	Scene(Scene&& other) = default;

	Scene& operator=(Scene const& other) = delete;

	Scene& operator=(Scene&& other) = default;

	virtual void frame(Seconds time);

private:
	typedef std::unordered_map<foonathan::string_id::string_id, Entity> EntityMap;

	foonathan::string_id::default_database _db;
	EntityMap _map;
};
