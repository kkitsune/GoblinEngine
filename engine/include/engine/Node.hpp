#pragma once

#include <ctti/type_id.hpp>
#include <forward_list>
#include <memory>
#include <string>
#include <ABI.h>

#include "Serializable.hpp"
#include "Math.hpp"

class E_ABI(engine) Component
{
	friend class Node;

public:
	virtual ~Component()
	{ }

protected:
	Component() : _node{}
	{ }

	std::weak_ptr<class Node> _node;
};

class E_ABI(engine) Node : public Serializable, public std::enable_shared_from_this<Node>
{
public:
	Node(std::string const& name = {});

	virtual ~Node();

	Node(Node const&) = delete;

	Node(Node&&) = default;

	Node& operator=(Node const&) = delete;

	Node& operator=(Node&&) = default;

	virtual Json save() override;

	virtual void load(Json const& js) override;

	inline std::shared_ptr<const Node> pointer() const
	{ return shared_from_this(); }

	inline std::shared_ptr<Node> pointer()
	{ return shared_from_this(); }

	inline std::shared_ptr<Node> parent() const
	{ return _parent.lock(); }

	inline std::shared_ptr<Node> parent()
	{ return _parent.lock(); }

	mat4 world_transform() const;

	mat4 transform() const;

	inline vec3 position() const
	{ return _pos; }

	inline void position(vec3 const& v)
	{ _pos = v; }

	inline vec3 scale() const
	{ return _scale; }

	inline void scale(vec3 const& v)
	{ _scale = v; }

	inline quat rotation() const
	{ return _rot; }

	inline void rotation(quat const& q)
	{ _rot = q; }

	std::shared_ptr<Node> create_child(std::string const& name = {});

protected:
	std::weak_ptr<Node> _parent;
	vec3 _pos, _scale;
	quat _rot;
	std::string _name;

	std::forward_list<std::pair<ctti::type_index, std::unique_ptr<Component>>> _components;
	std::forward_list<std::shared_ptr<Node>> _children;
};
