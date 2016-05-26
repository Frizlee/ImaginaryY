#include "..\include\VertexLayout.hpp"

VertexLayout::VertexLayout()
{
}

VertexLayout::~VertexLayout()
{
}

TextVertexLayout::Data::Data(float x, float y, float u, float v)
	: x{ x }, y{ y }, u{ u }, v{ v }
{
}

TextVertexLayout::TextVertexLayout()
{
}

TextVertexLayout::~TextVertexLayout()
{
}

std::uint32_t TextVertexLayout::Size()
{
	return sizeof(Data);
}


