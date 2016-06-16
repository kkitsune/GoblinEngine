#pragma once

#include <chrono>

using namespace std::chrono_literals;

using std::chrono::duration_cast;

typedef std::chrono::steady_clock Clock;
typedef Clock::time_point TimePoint;
typedef Clock::duration Duration;

typedef float Seconds;
inline Seconds as_seconds(Duration const& dT)
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(dT).count() / 1000.f;
}
