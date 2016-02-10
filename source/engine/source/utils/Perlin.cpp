#include <algorithm>
#include <ctime>

#include <engine/utils/Perlin.hpp>

using namespace std;
using namespace glm;

double fade(double t)
{
	return t * t * t * (t * (t * 6 - 15) + 10);
}

double lerp(double t, double a, double b)
{
	return a + t * (b - a);
}

double grad(int hash, double x, double y, double z)
{
	int h = hash & 15;
	double u = h < 8 ? x : y;
	double v = h < 4 ? y : h == 12 || h == 14 ? x : z;
	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

Perlin::Perlin(uint32 seed)
{
	if (seed == 0) seed = (uint32) time(0);
	auto mid_range = p.begin() + 256;

	mt19937 engine(seed);

	iota(p.begin(), mid_range, 0); //Generate sequential numbers in the lower half
	shuffle(p.begin(), mid_range, engine); //Shuffle the lower half
	copy(p.begin(), mid_range, mid_range); //Copy the lower half to the upper half
	//p now has the numbers 0-255, shuffled, and duplicated
}

Perlin::~Perlin()
{
}

double Perlin::noise(double x, double y, double z) const
{
	//See here for algorithm: http://cs.nyu.edu/~perlin/noise/
	const int32 X = static_cast<int32>(floor(x)) & 255;
	const int32 Y = static_cast<int32>(floor(y)) & 255;
	const int32 Z = static_cast<int32>(floor(z)) & 255;

	x -= floor(x);
	y -= floor(y);
	z -= floor(z);

	const double u = fade(x);
	const double v = fade(y);
	const double w = fade(z);

	const auto A = p[X] + Y;
	const auto AA = p[A] + Z;
	const auto AB = p[A + 1] + Z;
	const auto B = p[X + 1] + Y;
	const auto BA = p[B] + Z;
	const auto BB = p[B + 1] + Z;

	const auto PAA = p[AA];
	const auto PBA = p[BA];
	const auto PAB = p[AB];
	const auto PBB = p[BB];
	const auto PAA1 = p[AA + 1];
	const auto PBA1 = p[BA + 1];
	const auto PAB1 = p[AB + 1];
	const auto PBB1 = p[BB + 1];

	const auto a = lerp(v,
						lerp(u, grad(PAA, x, y, z), grad(PBA, x - 1, y, z)),
						lerp(u, grad(PAB, x, y - 1, z), grad(PBB, x - 1, y - 1, z))
	);

	const auto b = lerp(v,
						lerp(u, grad(PAA1, x, y, z - 1), grad(PBA1, x - 1, y, z - 1)),
						lerp(u, grad(PAB1, x, y - 1, z - 1), grad(PBB1, x - 1, y - 1, z - 1))
	);
	return lerp(w, a, b);
}
