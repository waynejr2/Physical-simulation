#pragma once

#include "include.h"

class Direct
{
public:
	vec2 beg, end, normal, direction, coordinates;
	float angle;

	Direct(vec2 ibeg, vec2 iend);

	float y_by_x(float x) const;
	float x_by_y(float y) const;
	float dist_to_vec(const vec2&) const;

	tuple<vec2, bool> cross(const Direct& dir) const;
	vec2 perpend_cross_from_v(const vec2&) const;

	bool out_of_x(float x) const;
	bool out_of_y(float y) const;
	bool out_of_p(vec2 v) const;

protected:
	float a, b, c, lx, rx, uy, dy;

	void init_param();
};


Direct::Direct(vec2 ibeg, vec2 iend) : beg{ibeg}, end{iend} {
	init_param();
}


void Direct::init_param() {
	a = beg.y - end.y;
	b = end.x - beg.x;
	c = -a * beg.x - b * beg.y;

	normal = vs::norm(vec2(a, b));

	coordinates = end - beg;

	direction = (end == beg) ? vs::zero : vs::norm(coordinates);

	angle = atan(coordinates.y / coordinates.x);

	lx = min(beg.x, end.x); rx = max(beg.x, end.x);
	uy = min(beg.y, end.y); dy = max(beg.y, end.y);
}

inline float Direct::y_by_x(float x) const {
	return -(a * x + c) / b;
}

inline float Direct::x_by_y(float y) const {
	return -(b * y + c) / a;
}

inline float Direct::dist_to_vec(const vec2& m) const {
	return abs(a*m.x + b*m.y + c) / sqrt(a*a + b*b);
}

inline vec2 Direct::perpend_cross_from_v(const vec2& p) const {
	// WARNING: not ready!
	float x1=p.x, y2=p.y, 
		  y1=y_by_x(p.x), x2=x_by_y(p.y);

	float k = ((p.x-x1) * (x2-x1) + (p.y-y1)*(y2-y1))/ (pow(x2-x1, 2) + pow(y2-y1, 2)),
		  x = x1 + k * (x2-x1),
		  y = y1 + k * (y2-y1);

	return vec2{x, y};
}


tuple<vec2, bool> Direct::cross(const Direct& dir) const {
	if (direction == dir.direction or dir.beg == dir.end) return { vs::zero, false };

	float den = a * dir.b - dir.a * b,
		x = -(c * dir.b - dir.c * b) / den,
		y = -(a * dir.c - dir.a * c) / den;

	vec2 cross = vec2{x, y};

	return { cross, !(out_of_p(cross) or dir.out_of_p(cross)) };
}

inline bool Direct::out_of_x(float x) const { return x < lx or x > rx; }
inline bool Direct::out_of_y(float y) const { return y < uy or y > dy; }
inline bool Direct::out_of_p(vec2 v) const { return out_of_x(v.x) or out_of_y(v.y); }
