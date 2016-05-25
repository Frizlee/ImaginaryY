#ifndef VERTEX_LAYOUTS_HPP
#define VERTEX_LAYOUTS_HPP
#include "Prerequisites.hpp"

struct TextVertex
{
	float x, y;
	float u, v;

	TextVertex(float x, float y, float u, float v) 
		: x{ x }, y{ y }, u{ u }, v{ v } {}
};

#endif // VERTEX_LAYOUTS_HPP

