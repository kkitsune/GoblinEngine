#pragma once

#include "Utils/BitmaskOperators.hpp"
#include "Object.hpp"

enum class Winding
{
	CCW = 1,
	CW = 2
};

enum class Solidity
{
	Solid = 1,
	Hole = 2
};

enum class LineCap
{
	Butt,
	Round,
	Square,
	Bevel,
	Miter
};

enum class Align
{
	Left = 1 << 0,
	Center = 1 << 1,
	Right = 1 << 2,

	Top = 1 << 3,
	Middle = 1 << 4,
	Bottom = 1 << 5,
	Baseline = 1 << 6
};
template<>
struct enable_bitmask_operators<Align>
{
	static const bool enable = true;
};

enum class ImageFlags
{
	GenerateMipmaps = 1 << 0,
	RepeatX = 1 << 1,
	RepeatY = 1 << 2,
	FlipY = 1 << 3,
	Premultiplied = 1 << 4
};
template<>
struct enable_bitmask_operators<ImageFlags>
{
	static const bool enable = true;
};

struct TextMetrics
{
	float acender, descender, line_height;
};

struct TextRow
{
	std::string text;
	float width, min_x, max_x;
};

struct Paint
{
	mat2x3 transform;
	vec2 extent;
	float radius, feather;
	vec4 inner_color, outer_color;
	int image;
};

class ENGINE_API Painter final : public Object
{
public:
	Painter();

	virtual ~Painter();

	Painter(Painter const& other) = delete;

	Painter(Painter&& other);

	Painter& operator=(Painter const& other) = delete;

	Painter& operator=(Painter&& other);

	void begin_frame(ivec2 const& size, float pixelratio);

	void cancel_frame();

	void end_frame();

	void save();

	void restore();

	void reset();

	void stroke_color(vec4 const& color);

	void stroke_paint(Paint const& paint);

	void fill_color(vec4 const& color);

	void fill_paint(Paint const& paint);

	void miter_limit(float limit);

	void stroke_width(float size);

	void line_cap(LineCap const& cap);

	void line_join(LineCap const& cap);

	void global_alpha(float alpha);

	void reset_transform();

	void transform(mat2x3 const& m);

	void translate(vec2 const& v);

	void rotate(float angle);

	void skew_x(float angle);

	void skew_y(float angle);

	void scale(vec2 const& v);

	mat2x3 current_transform();

	int create_image(std::string const& file, ImageFlags flags);

	ivec2 image_size(int id);

	void delete_image(int id);

	void scissor(vec2 const& pos, vec2 const& size);

	void intersect_scissor(vec2 const& pos, vec2 const& size);

	void reset_scissor();

	void begin_path();

	void move_to(vec2 const& pos);

	void line_to(vec2 const& pos);

	void bezier_to(vec2 const& c1, vec2 const& c2, vec2 const& pos);

	void quad_to(vec2 const& c, vec2 const& pos);

	void arc_to(vec2 const& v1, vec2 const& v2, float radius);

	void close_path();

	void path_winding(const Winding& dir);

	void arc(vec2 const& c, float r, vec2 const& a, Winding const& dir);

	void rect(vec2 const& pos, vec2 const& size);

	void rounded_rect(vec2 const& pos, vec2 const& size, float r);

	void ellipse(vec2 const& c, vec2 const& r);

	void circle(vec2 const& c, float r);

	void fill();

	void stroke();

	int create_font(std::string const& name, std::string const& file);

	int find_font(std::string const& name);

	void font_size(float size);

	void font_blur(float blur);

	void text_letter_spacing(float spacing);

	void text_line_height(float height);

	void text_align(Align align);

	void font_face(int id);

	void font_face(std::string const& name);

	float text(vec2 const& pos, std::string const& str);

	void text_box(vec2 const& pos, float width, std::string const& str);

	float text_bounds(vec2 const& pos, std::string const& str, vec2& min, vec2& max);

	void text_box_bounds(vec2 const& pos, float width, std::string const& str, vec2& min, vec2& max);

	TextMetrics const text_metrics();

	std::vector<TextRow> const text_break_lines(std::string const& str, float break_width, int max_rows);

	Paint linear_gradient(vec2 const& start, vec2 const& end, vec4 const& start_color, vec4 const& end_color);

	Paint box_gradien(vec2 const& pos, vec2 const& size, float radius, float feather, vec4 const& start_color, vec4 const& end_color);

	Paint radial_gradient(vec2 const& center, float in_radius, float out_radius, vec4 const& start_color, vec4 const& end_color);

	Paint image_pattern(vec2 const& origin, vec2 const& extent, float angle, int image, float alpha);

private:
	struct NVGcontext* _vg;
};
