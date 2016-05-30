#include "..\include\VertexLayout.hpp"
using namespace std;

VertexFormat::VertexFormat()
	: empty{ true }
{
}

VertexFormat::VertexFormat(GLint size, GLenum type, GLboolean normalized, GLsizei stride, 
	std::uint32_t pointer)
	: empty{ false }, size { size }, type{ type }, normalized{ normalized }, stride{ stride }, pointer{ pointer }
{
}

VertexLayout::VertexLayout()
{
}

VertexLayout::~VertexLayout()
{
}

const std::vector<VertexFormat>& VertexLayout::getFormat()
{
	return mFormat;
}

TextVertexLayout::Data::Data(float x, float y, float u, float v)
	: x{ x }, y{ y }, u{ u }, v{ v }
{
}

TextVertexLayout::TextVertexLayout()
{
	mFormat = {
		VertexFormat{ 2, gl::FLOAT, gl::FALSE_, static_cast<GLsizei>(Size()), offsetof(Data, x) },
		VertexFormat{ 2, gl::FLOAT, gl::FALSE_, static_cast<GLsizei>(Size()), offsetof(Data, u) } 
	};
}

TextVertexLayout::~TextVertexLayout()
{
}

std::uint32_t TextVertexLayout::Size()
{
	return sizeof(Data);
}

TextureVertexLayout::Data::Data(float x, float y, float z, float u, float v)
	: x{ x }, y{ y }, z{ z }, u{ u }, v{ v }
{
}

TextureVertexLayout::TextureVertexLayout()
{
	mFormat = {
		VertexFormat{ 3, gl::FLOAT, gl::FALSE_, static_cast<GLsizei>(Size()), offsetof(Data, x) },
		VertexFormat{ 2, gl::FLOAT, gl::FALSE_, static_cast<GLsizei>(Size()), offsetof(Data, u) }
	};
}

TextureVertexLayout::~TextureVertexLayout()
{
}

std::uint32_t TextureVertexLayout::Size()
{
	return sizeof(Data);
}

ColorVertexLayout::Data::Data(float x, float y, float z, std::uint32_t color)
	: x{ x }, y{ y }, z{ z }, color{ color }
{
}

ColorVertexLayout::ColorVertexLayout()
{
	mFormat = {
		VertexFormat{ 3, gl::FLOAT, gl::FALSE_, static_cast<GLsizei>(Size()), offsetof(Data, x) },
		VertexFormat{ 4, gl::UNSIGNED_BYTE, gl::TRUE_, static_cast<GLsizei>(Size()), offsetof(Data, color) }
	};
}

ColorVertexLayout::~ColorVertexLayout()
{
}

std::uint32_t ColorVertexLayout::Size()
{
	return sizeof(Data);
}
