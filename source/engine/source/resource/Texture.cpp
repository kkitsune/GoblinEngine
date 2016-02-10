#include <engine/resource/Texture.hpp>
#include <engine/Core.hpp>

#include "../nanovg/stb_image.h"

std::vector<unsigned char> load_texture(std::string const& filename, glm::ivec2& size, int& components)
{
	auto texture_path = Core::instance().settings().texture_path();
	unsigned char* data = stbi_load((texture_path / filename).string().c_str(), &size.x, &size.y, &components, 4);
	if(data == nullptr) throw std::runtime_error("Could not load Texture : " + (texture_path / filename).generic_string());
	std::vector<unsigned char> ret;
	for (int i = 0; i < size.x * size.y * components; i++)
		ret.push_back(data[i]);
	stbi_image_free(data);
	return ret;
}
