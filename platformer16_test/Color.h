/*

A simple struct which holds RGBA8 color information
*/

#pragma once

struct Color
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
	Color() : r(0), g(0), b(0), a(255) {}
	Color(unsigned char _r, unsigned char _g, unsigned char _b) : r(_r), g(_g), b(_b), a(255) {}
	Color(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a) : r(_r), g(_g), b(_b), a(_a) {}
};