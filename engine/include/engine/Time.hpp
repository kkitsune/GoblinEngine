#pragma once

#include <chrono>

typedef unsigned int FramesPerSecond;
typedef float Seconds;

inline Seconds time_now()
{
	using namespace std;
	return chrono::duration_cast<chrono::duration<Seconds, ratio<1>>>(chrono::high_resolution_clock::now().time_since_epoch()).count();
}
