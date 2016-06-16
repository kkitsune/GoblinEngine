#include <glm/gtc/matrix_transform.hpp>
#include <engine/scene/Node.hpp>

using namespace glm;
using namespace std;

Node::Transform::Transform() : position{0, 0, 0}, scale{1, 1, 1}, rotation{}
{ }

mat4 Node::Transform::calculate() const
{
	mat4 ret = ::translate(mat4(1.f), position);
	ret *= mat4_cast(rotation);
	ret *= ::scale(mat4(1.f), scale);
	return ret;
}

Node::Node() : _parent{}, _name{}, _children{}
{ }

Node::~Node()
{
	for(auto c : _components)
		delete c.second;
	_components.clear();
}

void Node::add_child(string const& name, shared_ptr<Node> const& node)
{
	node->parent(pointer());
	node->name(name);
	_children.push_front(node);
}
