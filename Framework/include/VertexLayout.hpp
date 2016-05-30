#ifndef VERTEX_LAYOUT_HPP
#define VERTEX_LAYOUT_HPP
#include "Prerequisites.hpp"
#include "gl_core_3_3.hpp"

struct VertexFormat
{
	bool empty;
	GLint size;
	GLenum type;
	GLboolean normalized;
	GLsizei stride;
	std::uint32_t pointer;

	VertexFormat();
	VertexFormat(GLint size, GLenum type, GLboolean normalized, GLsizei stride, std::uint32_t pointer);
};

class VertexLayout
{
public:
	VertexLayout();
	virtual ~VertexLayout() = 0;

	const std::vector<VertexFormat>& getFormat();

protected:
	std::vector<VertexFormat> mFormat;
private:
};

class TextVertexLayout : public VertexLayout
{
public:
	struct Data
	{
		float x, y;
		float u, v;

		Data(float x, float y, float u, float v);
	};
	
	TextVertexLayout();
	~TextVertexLayout();

	static std::uint32_t Size();

private:
};

class TextureVertexLayout : public VertexLayout
{
public:
	struct Data
	{
		float x, y, z;
		float u, v;

		Data(float x, float y, float z, float u, float v);
	};

	TextureVertexLayout();
	~TextureVertexLayout();

	static std::uint32_t Size();

private:
};

class ColorVertexLayout : public VertexLayout
{
public:
	struct Data
	{
		float x, y, z;
		std::uint32_t color;

		Data(float x, float y, float z, std::uint32_t color);
	};

	ColorVertexLayout();
	~ColorVertexLayout();

	static std::uint32_t Size();

private:
};

#endif // VERTEX_LAYOUT_HPP

