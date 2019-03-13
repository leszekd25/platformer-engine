#pragma once

#include <iostream>

const int CELL_SIZE = 24;

struct Tilepos
{
	int x, y;
	Tilepos() : x(0), y(0) {}
	Tilepos(int _x, int _y) : x(_x), y(_y) {}
	Tilepos& operator+=(const Tilepos& v)
	{
		x += v.x;
		y += v.y;
		return *this;
	}
	Tilepos& operator-=(const Tilepos& v)
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}
	friend Tilepos operator-(Tilepos v)
	{
		return Tilepos(-v.x, -v.y);
	}
	friend Tilepos operator+(Tilepos v1, const Tilepos& v2)
	{
		v1 += v2;
		return v1;
	}
	friend Tilepos operator-(Tilepos v1, const Tilepos& v2)
	{
		v1 -= v2;
		return v1;
	}
	friend bool operator==(Tilepos t1, const Tilepos& t2)
	{
		return ((t1.x == t2.x) && (t1.y == t2.y));
	}
	friend bool operator!=(Tilepos t1, const Tilepos& t2)
	{
		return ((t1.x != t2.x) || (t1.y != t2.y));
	}
	friend std::ostream& operator<<(std::ostream& os, const Tilepos& r)
	{
		os << "(" << r.x << ", " << r.y << ")";;
		return os;
	}
};