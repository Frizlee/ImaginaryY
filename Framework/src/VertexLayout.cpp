#include "..\include\VertexLayout.hpp"
using namespace std;

std::vector<VertexAttrib> TextVertexLayout::mAttrib{
	VertexAttrib{ 2, gl::FLOAT, gl::FALSE_, static_cast<GLsizei>(Size()), 0 },
	VertexAttrib{ 2, gl::FLOAT, gl::FALSE_, static_cast<GLsizei>(Size()), 2 * sizeof(float) }
};

std::vector<VertexAttrib> TextureVertexLayout::mAttrib{
	VertexAttrib{ 3, gl::FLOAT, gl::FALSE_, static_cast<GLsizei>(Size()), 0 },
	VertexAttrib{ 2, gl::FLOAT, gl::FALSE_, static_cast<GLsizei>(Size()), 3 * sizeof(float) }
};

VertexAttrib::VertexAttrib(GLint size, GLenum type, GLboolean normalized, GLsizei stride, 
	std::uint32_t pointer)
	: size{ size }, type{ type }, normalized{ normalized }, stride{ stride }, pointer{ pointer }
{
}

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

const std::vector<VertexAttrib>& TextVertexLayout::GetAttrib()
{
	return mAttrib;
}

TextureVertexLayout::TextureVertexLayout()
{
}

TextureVertexLayout::~TextureVertexLayout()
{
}

std::uint32_t TextureVertexLayout::Size()
{
	return sizeof(Data);
}

const std::vector<VertexAttrib>& TextureVertexLayout::GetAttrib()
{
	return mAttrib;
}

TextureVertexLayout::Data::Data(float x, float y, float z, float u, float v)
	: x{ x }, y{ y }, z{ z }, u{ u }, v{ v }
{
}
