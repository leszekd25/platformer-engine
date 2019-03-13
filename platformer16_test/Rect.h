/*

A structure which stores information about a certain axis-aligned rectangle in 2-dimentional space
Contains method for testing whether a rectangle intersects with another rectangle, and if it contains a given point
*/

#pragma once

#include <cmath>
#include <iostream>
#include "Vector.h"

// stored as center and half-dimensions
struct Rect
{
	// center of rectangle, half-dimensions of rectangle
	float x, y, w2, h2;
	Rect() : x(0), y(0), w2(0), h2(0) {}
	// arguments: top-left x/y, full width/height
	Rect(float _x, float _y, float _w, float _h)
	{
		w2 = _w / 2;
		h2 = _h / 2;
		x = _x + w2;
		y = _y + h2;
	}
	// arguments: two points in space
	// this constructor will automatically find a rectangle constrained by those two points
	Rect(const Vector& v1, const Vector& v2)
	{
		float x1 = (v1.x < v2.x ? v1.x : v2.x);
		float y1 = (v1.y < v2.y ? v1.y : v2.y);
		w2 = abs(v1.x - v2.x)/2;
		h2 = abs(v1.y - v2.y)/2;
		x = x1 + w2;
		y = y1 + h2;
	}
	Rect& operator+=(const Vector& v)
	{
		x += v.x; y += v.y;
		return *this;
	}
	Rect& operator-=(const Vector& v)
	{
		x -= v.x; y -= v.y;
		return *this;
	}
	friend Rect operator+(Rect r, const Vector& v)
	{
		r += v;
		return r;
	}
	friend Rect operator-(Rect r, const Vector& v)
	{
		r -= v;
		return r;
	}
	friend std::ostream& operator<<(std::ostream& os, const Rect& r)
	{
		os << "[(" << r.x - r.w2 << ", " << r.y - r.w2 << "), (" << r.w2 * 2 << ", " << r.h2 * 2 << ")]";
		return os;
	}

	bool Contains(const Vector& v)
	{
		return (abs(v.x - x) < w2) && (abs(v.y - y) < h2);
	}

	bool Intersects(const Rect& r)
	{
		return (abs(x - r.x) < (w2 + r.w2)) &&
			(abs(y - r.y) < (h2 + r.h2));
	}

	float ManhattanDistance(const Rect& r)
	{
		float f1 = abs(x - r.x) - (w2 + r.w2);
		float f2 = abs(y - r.y) - (h2 + r.h2);
		return (f1 > f2 ? f1 : f2);
	}

	float ManhattanDistance(const Vector& v)
	{
		float f1 = abs(x - v.x) - (w2);
		float f2 = abs(y - v.y) - (h2);
		return (f1 > f2 ? f1 : f2);
	}
};