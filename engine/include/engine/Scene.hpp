#pragma once

#include <engine/config.h>
#include <engine/Time.hpp>

#include <glm/glm.hpp>

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
};
