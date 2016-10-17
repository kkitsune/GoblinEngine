#pragma once

#include <json.hpp>
#include <ABI.h>

using Json = nlohmann::basic_json<>;

class E_ABI(engine) Serializable
{
public:
	virtual ~Serializable()
	{ }

	virtual Json save() = 0;

	virtual void load(Json const&) = 0;
};
