#include <boost/filesystem.hpp>
#include <engine/Painter.hpp>
#include <glad/glad.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#define NANOVG_GL3_IMPLEMENTATION
#include "deps/nanovg/nanovg.h"
#include "deps/nanovg/nanovg_gl.h"
#pragma GCC diagnostic pop

using namespace glm;
using namespace std;

NVGcolor nano_color(vec4 const& v)
{
	NVGcolor ret;
	ret.r = v.r;
	ret.g = v.g;
	ret.b = v.b;
	ret.a = v.a;
	return ret;
}

NVGpaint to_nvg(Paint const& paint)
{
	return {
		{
			paint.transform[0].x,
			paint.transform[1].x,
			paint.transform[0].y,
			paint.transform[1].y,
			paint.transform[0].z,
			paint.transform[1].z
		},
		{paint.extent.x, paint.extent.y},
		paint.radius,
		paint.feather,
		nano_color(paint.inner_color),
		nano_color(paint.outer_color),
		paint.image
	};
}

Paint from_nvg(NVGpaint const& paint)
{
	return {
		mat2x3(paint.xform[0], paint.xform[2], paint.xform[4], paint.xform[1], paint.xform[3], paint.xform[5]),
		vec2(paint.extent[0], paint.extent[1]),
		paint.radius,
		paint.feather,
		vec4(paint.innerColor.r, paint.innerColor.g, paint.innerColor.b, paint.innerColor.a),
		vec4(paint.outerColor.r, paint.outerColor.g, paint.outerColor.b, paint.outerColor.a),
		paint.image
	};
}

Painter::Painter() : _vg(nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG))
{ }

Painter::~Painter()
{
	nvgDeleteGL3(_vg);
	_vg = nullptr;
}

void Painter::begin_frame(ivec2 const& size, float pixelratio)
{
	nvgBeginFrame(_vg, size.x, size.y, pixelratio);
}

void Painter::cancel_frame()
{
	nvgCancelFrame(_vg);
}

void Painter::end_frame()
{
	nvgEndFrame(_vg);
}

void Painter::save()
{
	nvgSave(_vg);
}

void Painter::restore()
{
	nvgRestore(_vg);
}

void Painter::reset()
{
	nvgReset(_vg);
}

void Painter::stroke_color(vec4 const& v)
{
	nvgStrokeColor(_vg, nano_color(v));
}

void Painter::stroke_paint(Paint const& paint)
{
	nvgStrokePaint(_vg, to_nvg(paint));
}

void Painter::fill_color(vec4 const& v)
{
	nvgFillColor(_vg, nano_color(v));
}

void Painter::fill_paint(Paint const& paint)
{
	nvgFillPaint(_vg, to_nvg(paint));
}

void Painter::miter_limit(float limit)
{
	nvgMiterLimit(_vg, limit);
}

void Painter::stroke_width(float size)
{
	nvgStrokeWidth(_vg, size);
}

void Painter::line_cap(LineCap const& cap)
{
	if(cap != LineCap::Butt && cap != LineCap::Round && cap != LineCap::Square) throw std::invalid_argument("Line Cap can only be Butt, Round or Square");
	nvgLineCap(_vg, static_cast<int>(cap));
}

void Painter::line_join(LineCap const& cap)
{
	if(cap != LineCap::Miter && cap != LineCap::Round && cap != LineCap::Bevel) throw std::invalid_argument("Line Join can only be Miter, Round or Bevel");
	nvgLineJoin(_vg, static_cast<int>(cap));
}

void Painter::global_alpha(float alpha)
{
	nvgGlobalAlpha(_vg, alpha);
}

void Painter::reset_transform()
{
	nvgResetTransform(_vg);
}

void Painter::transform(mat2x3 const& m)
{
	nvgTransform(_vg, m[0].x, m[1].x, m[0].y, m[1].y, m[0].z, m[1].z);
}

void Painter::translate(vec2 const& v)
{
	nvgTranslate(_vg, v.x, v.y);
}

void Painter::rotate(float angle)
{
	nvgRotate(_vg, angle);
}

void Painter::skew_x(float angle)
{
	nvgSkewX(_vg, angle);
}

void Painter::skew_y(float angle)
{
	nvgSkewY(_vg, angle);
}

void Painter::scale(vec2 const& v)
{
	nvgScale(_vg, v.x, v.y);
}

mat2x3 Painter::current_transform()
{
	float m[6];
	nvgCurrentTransform(_vg, m);
	return mat2x3(m[0], m[2], m[4], m[1], m[3], m[5]);
}

int Painter::create_image(string const& file, ImageFlags flags)
{
	if(!boost::filesystem::exists(file)) throw std::invalid_argument("Image file '" + file + "' does not exist");
	return nvgCreateImage(_vg, file.c_str(), static_cast<int>(flags));
}

int Painter::create_image(unsigned char const* data, int size, ImageFlags flags)
{
	return nvgCreateImageMem(_vg, static_cast<int>(flags), data, size);
}

ivec2 Painter::image_size(int id)
{
	ivec2 ret;
	nvgImageSize(_vg, id, &ret.x, &ret.y);
	return ret;
}

void Painter::delete_image(int id)
{
	nvgDeleteImage(_vg, id);
}

void Painter::scissor(vec2 const& pos, vec2 const& size)
{
	nvgScissor(_vg, pos.x, pos.y, size.x, size.y);
}

void Painter::intersect_scissor(vec2 const& pos, vec2 const& size)
{
	nvgIntersectScissor(_vg, pos.x, pos.y, size.x, size.y);
}

void Painter::reset_scissor()
{
	nvgResetScissor(_vg);
}

void Painter::begin_path()
{
	nvgBeginPath(_vg);
}

void Painter::move_to(vec2 const& pos)
{
	nvgMoveTo(_vg, pos.x, pos.y);
}

void Painter::line_to(vec2 const& pos)
{
	nvgLineTo(_vg, pos.x, pos.y);
}

void Painter::bezier_to(vec2 const& c1, vec2 const& c2, vec2 const& pos)
{
	nvgBezierTo(_vg, c1.x, c1.y, c2.x, c2.y, pos.x, pos.y);
}

void Painter::quad_to(vec2 const& c, vec2 const& pos)
{
	nvgQuadTo(_vg, c.x, c.y, pos.x, pos.y);
}

void Painter::arc_to(vec2 const& v1, vec2 const& v2, float radius)
{
	nvgArcTo(_vg, v1.x, v1.y, v2.x, v2.y, radius);
}

void Painter::close_path()
{
	nvgClosePath(_vg);
}

void Painter::path_winding(const Winding& dir)
{
	nvgPathWinding(_vg, static_cast<int>(dir));
}

void Painter::arc(vec2 const& c, float r, vec2 const& a, Winding const& dir)
{
	nvgArc(_vg, c.x, c.y, r, a.x, a.y, static_cast<int>(dir));
}

void Painter::rect(vec2 const& pos, vec2 const& size)
{
	nvgRect(_vg, pos.x, pos.y, size.x, size.y);
}

void Painter::rounded_rect(vec2 const& pos, vec2 const& size, float r)
{
	nvgRoundedRect(_vg, pos.x, pos.y, size.x, size.y, r);
}

void Painter::ellipse(vec2 const& c, vec2 const& r)
{
	nvgEllipse(_vg, c.x, c.y, r.x, r.y);
}

void Painter::circle(vec2 const& c, float r)
{
	nvgCircle(_vg, c.x, c.y, r);
}

void Painter::fill()
{
	nvgFill(_vg);
}

void Painter::stroke()
{
	nvgStroke(_vg);
}

int Painter::create_font(string const& name, string const& file)
{
	if(!boost::filesystem::exists(file)) throw std::invalid_argument("Font file '" + file + "' does not exist");
	return nvgCreateFont(_vg, name.c_str(), file.c_str());
}

int Painter::find_font(string const& name)
{
	return nvgFindFont(_vg, name.c_str());
}

void Painter::font_size(float size)
{
	nvgFontSize(_vg, size);
}

void Painter::font_blur(float blur)
{
	nvgFontBlur(_vg, blur);
}

void Painter::text_letter_spacing(float spacing)
{
	nvgTextLetterSpacing(_vg, spacing);
}

void Painter::text_line_height(float height)
{
	nvgTextLineHeight(_vg, height);
}

void Painter::text_align(Align align)
{
	nvgTextAlign(_vg, static_cast<int>(align));
}

void Painter::font_face(int id)
{
	nvgFontFaceId(_vg, id);
}

void Painter::font_face(string const& name)
{
	nvgFontFace(_vg, name.c_str());
}

float Painter::text(vec2 const& pos, string const& str)
{
	return nvgText(_vg, pos.x, pos.y, str.c_str(), 0);
}

void Painter::text_box(vec2 const& pos, float width, string const& str)
{
	nvgTextBox(_vg, pos.x, pos.y, width, str.c_str(), 0);
}

float Painter::text_bounds(vec2 const& pos, string const& str, vec2& min, vec2& max)
{
	float b[4];
	float ret = nvgTextBounds(_vg, pos.x, pos.y, str.c_str(), 0, b);
	min.x = b[0];
	min.y = b[1];
	max.x = b[2];
	max.y = b[3];
	return ret;
}

void Painter::text_box_bounds(vec2 const& pos, float width, string const& str, vec2& min, vec2& max)
{
	float b[4];
	nvgTextBoxBounds(_vg, pos.x, pos.y, width, str.c_str(), 0, b);
	min.x = b[0];
	min.y = b[1];
	max.x = b[2];
	max.y = b[3];
}

TextMetrics const Painter::text_metrics()
{
	TextMetrics ret;
	nvgTextMetrics(_vg, &ret.acender, &ret.descender, &ret.line_height);
	return ret;
}

vector<TextRow> const Painter::text_break_lines(string const& str, float break_width, int max_rows)
{
	const char* data = str.data();
	vector<TextRow> ret;
	NVGtextRow rows[256];
	int size = nvgTextBreakLines(_vg, data, 0, break_width, rows, max_rows);
	for (int i = 0; i < size; i++)
	{
		TextRow tr;
		tr.max_x = rows[i].maxx;
		tr.min_x = rows[i].minx;
		tr.width = rows[i].width;
		tr.text = string(rows[i].start, rows[i].end - rows[i].start);
		ret.push_back(tr);
	}
	return ret;
}

Paint Painter::linear_gradient(vec2 const& start, vec2 const& end, vec4 const& start_color, vec4 const& end_color)
{
	return from_nvg(nvgLinearGradient(_vg, start.x, start.y, end.x, end.y,
									  nano_color(start_color), nano_color(end_color)));
}

Paint Painter::box_gradien(vec2 const& pos, vec2 const& size, float radius, float feather, vec4 const& start_color, vec4 const& end_color)
{
	return from_nvg(nvgBoxGradient(_vg, pos.x, pos.y, size.x, size.y, radius, feather,
								   nano_color(start_color), nano_color(end_color)));
}

Paint Painter::radial_gradient(vec2 const& center, float in_radius, float out_radius, vec4 const& start_color, vec4 const& end_color)
{
	return from_nvg(nvgRadialGradient(_vg, center.x, center.y, in_radius, out_radius,
									  nano_color(start_color), nano_color(end_color)));
}

Paint Painter::image_pattern(vec2 const& origin, vec2 const& extent, float angle, int image, float alpha)
{
	return from_nvg(nvgImagePattern(_vg, origin.x, origin.y, extent.x, extent.y, angle, image, alpha));
}
