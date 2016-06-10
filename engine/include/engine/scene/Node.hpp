#pragma once

#include <engine/config.h>

#include <glm/gtc/quaternion.hpp>
#include <ctti/type_id.hpp>
#include <forward_list>
#include <type_traits>
#include <glm/glm.hpp>
#include <functional>
#include <memory>
#include <string>

class ENGINE_API Node : private std::enable_shared_from_this<Node>
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

	Node();

	virtual ~Node();

	Node(Node const& other) = delete;

	Node(Node&& other) = default;

	Node& operator=(Node const& other) = delete;

	Node& operator=(Node&& other) = default;

	inline std::shared_ptr<const Node> pointer() const
	{ return shared_from_this(); }

	inline std::shared_ptr<Node> pointer()
	{ return shared_from_this(); }

	virtual inline std::string type_name() const
	{ return ctti::type_id<Node>().name().c_str(); }

	virtual inline Transform const& transform() const
	{ return _transform; }

	virtual inline Transform& transform()
	{ return _transform; }

	virtual inline std::string const& name() const
	{ return _name; }

	virtual inline void name(std::string const& name)
	{ _name = name; }

	virtual inline std::weak_ptr<const Node> parent() const
	{ return _parent; }

	virtual inline std::weak_ptr<Node> parent()
	{ return _parent; }

	virtual inline void parent(std::weak_ptr<Node> const& node)
	{ _parent = node; }

	virtual inline std::forward_list<std::shared_ptr<Node>> const& children() const
	{ return _children; }

	virtual inline std::forward_list<std::shared_ptr<Node>>& children()
	{ return _children; }

	template<class NodeT, typename... Args>
	inline std::shared_ptr<NodeT> add_child(std::string const& name, Args&& ... args)
	{
		static_assert(std::is_base_of<Node, NodeT>::value, "Node Type must be a subclass of Node");
		static_assert(std::is_constructible<NodeT, Args...>::value, "Node Type must have a matching constructor");
		auto child = std::make_shared<NodeT>(std::forward<Args>(args)...);
		add_child(name, std::dynamic_pointer_cast<Node>(child));
		return child;
	}

	inline void accept(std::function<void(std::shared_ptr<Node> const&)> const& visitor)
	{
		for(auto child : _children)
			child->accept(visitor);

		visitor(pointer());
	}

	virtual inline void begin_frame()
	{ for (auto child : _children) child->begin_frame(); }

	virtual inline void draw()
	{ for (auto child : _children) child->draw(); }

	virtual inline void end_frame()
	{ for (auto child : _children) child->end_frame(); }

protected:
	virtual void add_child(std::string const& name, std::shared_ptr<Node> const& node);

	std::weak_ptr<Node> _parent;
	std::string _name;
	Transform _transform;

	std::forward_list<std::shared_ptr<Node>> _children;
};
