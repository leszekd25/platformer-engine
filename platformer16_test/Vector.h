/*
A minimal 2-dimensional vector class, to be developed at later date

Usage:

Vector v(3, 4);
Vector u(6, -2);
std::cout << v*4.5 << std::endl;    // (13.5, 18)
assert(v+u == Vector(9, 2));
assert(V-u == Vector(-3, 6));

*/

#pragma once

#include <cmath>
#include <iostream>

struct Vector
{
	float x, y;
	Vector() : x(.0f), y(.0f) {}
	Vector(float _x, float _y) : x(_x), y(_y) {}
	Vector& operator+=(const Vector& v)
	{
		x += v.x;
		y += v.y;
		return *this;
	}
	Vector& operator-=(const Vector& v)
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}
	Vector& operator*=(float f)
	{
		x *= f;
		y *= f;
		return *this;
	}
	friend Vector operator-(Vector v)
	{
		return Vector(-v.x, -v.y);
	}
	friend Vector operator+(Vector v1, const Vector& v2)
	{
		v1 += v2;
		return v1;
	}
	friend Vector operator-(Vector v1, const Vector& v2)
	{
		v1 -= v2;
		return v1;
	}
	friend Vector operator*(Vector v, float f)
	{
		v *= f;
		return v;
	}
	friend bool operator==(Vector v1, const Vector& v2)
	{
		return ((v1.x == v2.x) && (v1.y == v2.y));
	}
	friend bool operator!=(Vector v1, const Vector& v2)
	{
		return ((v1.x != v2.x) || (v1.y != v2.y));
	}
	friend std::ostream& operator<<(std::ostream& os, const Vector& r)
	{
		os << "(" << r.x << ", " << r.y << ")";;
		return os;
	}

	float Distance(Vector v)
	{
		return sqrt((x - v.x)*(x - v.x) + (y - v.y)*(y - v.y));
	}

	float Distance2(Vector v)
	{
		return (x - v.x)*(x - v.x) + (y - v.y)*(y - v.y);
	}

	float ManhattanDistance(Vector v)
	{
		float f1 = abs(x - v.x);
		float f2 = abs(y - v.y);
		return (f1 > f2 ? f1 : f2);
	}
};

