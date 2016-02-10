#pragma once

#include <glm/glm.hpp>
#include <random>
#include <array>

#include "../Common.hpp"

// from http://blog.kazade.co.uk/2014/05/a-public-domain-c11-1d2d3d-perlin-noise.html?m=1
class ENGINE_API Perlin
{
public:
	Perlin(glm::uint32 seed = 0);

	virtual ~Perlin();

	Perlin(Perlin const& other) = delete;

	Perlin(Perlin&& other) = delete;

	Perlin& operator=(Perlin const& other) = delete;

	Perlin& operator=(Perlin&& other) = delete;

	double noise(double x) const
	{ return noise(x, 0, 0); }

	double noise(double x, double y) const
	{ return noise(x, y, 0); }

	double noise(double x, double y, double z) const;

protected:
	std::array<int, 512> p;
};
