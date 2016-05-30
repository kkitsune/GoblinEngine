#include <engine/Blendish.hpp>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#define BLENDISH_IMPLEMENTATION
#include "deps/nanovg/blender_icons.h"
#include "deps/nanovg/nanovg.h"
#include "deps/nanovg/blendish.h"
#pragma GCC diagnostic pop

Blendish::Blendish(Painter& painter) : _vg(painter._vg)
{
	bndSetIconImage(painter.create_image(blender_icons, sizeof(blender_icons), ImageFlags::None));
	bndSetFont(painter.create_font("dejavu", "data/DejaVuSans.ttf"));
}

Blendish::~Blendish()
{
}
