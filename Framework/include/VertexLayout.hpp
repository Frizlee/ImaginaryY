#ifndef VERTEX_LAYOUT_HPP
#define VERTEX_LAYOUT_HPP
#include "Prerequisites.hpp"
#include "gl_core_3_3.hpp"

struct VertexAttrib
{
	GLint size;
	GLenum type;
	GLboolean normalized;
	GLsizei stride;
	std::uint32_t pointer;

	VertexAttrib(GLint size, GLenum type, GLboolean normalized, GLsizei stride, std::uint32_t pointer);
};

class VertexLayout
{
public:
	VertexLayout();
	virtual ~VertexLayout() = 0;

protected:

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
	static const std::vector<VertexAttrib>& GetAttrib();

private:
	static std::vector<VertexAttrib> mAttrib;
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
	static const std::vector<VertexAttrib>& GetAttrib();

private:
	static std::vector<VertexAttrib> mAttrib;
};

#endif // VERTEX_LAYOUT_HPP

