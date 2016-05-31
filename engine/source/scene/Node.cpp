#include <glm/gtc/matrix_transform.hpp>
#include <engine/scene/Node.hpp>

using namespace glm;
using namespace std;

Node::Transform::Transform() : position{0, 0, 0}, scale{1, 1, 1}, rotation{}
{ }

mat4 Node::Transform::calculate() const
{
	mat4 ret = glm::translate(mat4(1.f), position);
	ret *= mat4_cast(rotation);
	ret *= glm::scale(mat4(1.f), scale);
	return ret;
}

Node::Node() : _parent{}, _name{}, _children{}
{ }

Node::~Node()
{ }

void Node::add_child(std::string const& name, std::shared_ptr<Node> const& node)
{
	node->parent(shared_from_this());
	node->name(name);
	_children.push_front(node);
}
