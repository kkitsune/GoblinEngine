#include <engine/Engine.hpp>
#include <engine/Node.hpp>

Node::Node() : _parent{}, _pos{0.f, 0.f, 0.f}, _scale{1.f, 1.f, 1.f}, _rot{1.f, 0.f, 0.f, 0.f}
{ }

Node::~Node()
{ }

Json Node::save()
{
	Json ret = Json::object({});
	ret["position"] = Json::array(
			{(real::floating_point_type) _pos.x,
			 (real::floating_point_type) _pos.y,
			 (real::floating_point_type) _pos.z}
	);
	ret["scale"] = Json::array(
			{(real::floating_point_type) _scale.x,
			 (real::floating_point_type) _scale.y,
			 (real::floating_point_type) _scale.z}
	);
	ret["rotation"] = Json::array(
			{(real::floating_point_type) _rot.x,
			 (real::floating_point_type) _rot.y,
			 (real::floating_point_type) _rot.z,
			 (real::floating_point_type) _rot.w}
	);

	return ret;
}

void Node::load(Json const& js)
{
	if(!js.is_object())
		throw std::runtime_error("Node => Could not load Json : Json is not an object");
}

mat4 Node::world_transform() const
{
	mat4 ret{1.f};
	if(auto parent = _parent.lock())
		ret *= parent->world_transform();

	return ret * transform();
}

mat4 Node::transform() const
{
	return glm::translate(mat4(1.f), _pos) * glm::mat4_cast(_rot) * glm::scale(mat4(1.f), _scale);
}
