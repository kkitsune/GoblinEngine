#pragma once

#include <engine/config.h>

#include <glm/gtc/quaternion.hpp>
#include <ctti/type_id.hpp>
#include <unordered_map>
#include <forward_list>
#include <type_traits>
#include <glm/glm.hpp>
#include <functional>
#include <stdexcept>
#include <memory>
#include <string>

struct Component
{
	virtual ~Component()
	{ }

	std::weak_ptr<class Node> node;
};

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

	virtual inline std::shared_ptr<Node> parent() const
	{ return _parent.lock(); }

	virtual inline void parent(std::shared_ptr<Node> const& node)
	{ _parent = node; }

	virtual inline std::forward_list<std::shared_ptr<Node>> const& children() const
	{ return _children; }

	virtual inline std::forward_list<std::shared_ptr<Node>>& children()
	{ return _children; }

	inline std::shared_ptr<Node> create_child(std::string const& name)
	{
		auto child = std::make_shared<Node>();
		add_child(name, child);
		return child;
	}

	virtual void add_child(std::string const& name, std::shared_ptr<Node> const& node);

	template<typename CompT, typename... Args>
	inline CompT& create_component(Args&& ... args) throw(std::invalid_argument)
	{
		static_assert(std::is_base_of<Component, CompT>::value, "Component Type must be a subclass of Component");
		static_assert(std::is_constructible<CompT, Args...>::value, "Component Type must have a matching constructor");
		if(_components.find(ctti::type_id<CompT>()) == _components.end())
		{
			CompT* ret = new CompT(std::forward<Args>(args)...);
			_components[ctti::type_id<CompT>()] = ret;
			return *ret;
		}

		throw std::invalid_argument("Component already exists");
	}

	template<typename CompT>
	inline bool has_component() const
	{ return _components.find(ctti::type_id<CompT>()) != _components.end(); }

	template<typename CompT>
	inline CompT const& component() const throw(std::invalid_argument)
	{
		if(_components.find(ctti::type_id<CompT>()) == _components.end())
			return *(_components.at(ctti::type_id<CompT>()));

		throw std::invalid_argument("Component does not exist");
	}

	template<typename CompT>
	inline CompT& component() throw(std::invalid_argument)
	{
		if(_components.find(ctti::type_id<CompT>()) == _components.end())
			return *(_components.at(ctti::type_id<CompT>()));

		throw std::invalid_argument("Component does not exist");
	}

	inline void accept(std::function<void(std::shared_ptr<Node> const&)> const& visitor)
	{
		for(auto child : _children)
			child->accept(visitor);

		visitor(pointer());
	}

protected:
	std::weak_ptr<Node> _parent;
	std::string _name;
	Transform _transform;

	std::unordered_map<ctti::type_index, Component*> _components;
	std::forward_list<std::shared_ptr<Node>> _children;
};
