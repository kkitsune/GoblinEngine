#pragma once

#include <engine/config.h>

#include <glm/gtc/quaternion.hpp>
#include <glm/glm.hpp>
#include <functional>
#include <memory>
#include <list>

class ENGINE_API Node : public std::enable_shared_from_this<Node>
{
public:
	struct Transform
	{
		Transform();

		Transform(Transform const& other) = default;

		Transform(Transform&& other) = default;

		Transform& operator=(Transform const& other) = default;

		Transform& operator=(Transform&& other) = default;

		glm::mat4 calculate() const;

		glm::vec3 position, scale;
		glm::quat rotation;
	};

	Node(std::weak_ptr<Node> const& parent);

	virtual ~Node();

	Node(Node const& other) = delete;

	Node(Node&& other) = default;

	Node& operator=(Node const& other) = delete;

	Node& operator=(Node&& other) = default;

	virtual inline Transform const& transform() const
	{ return _transform; }

	virtual inline Transform& transform()
	{ return _transform; }

protected:
	std::weak_ptr<Node> _parent;
	Transform _transform;
};
