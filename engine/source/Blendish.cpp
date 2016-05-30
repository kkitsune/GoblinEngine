#include <engine/Blendish.hpp>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#define BLENDISH_IMPLEMENTATION
#include "deps/nanovg/nanovg.h"
#include "deps/nanovg/blendish.h"
#pragma GCC diagnostic pop

Blendish::Blendish(Painter& painter) : _vg(painter._vg)
{
}

Blendish::~Blendish()
{
}
