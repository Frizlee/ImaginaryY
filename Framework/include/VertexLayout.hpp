#ifndef VERTEX_LAYOUT_HPP
#define VERTEX_LAYOUT_HPP
#include "Prerequisites.hpp"

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

private:
};

#endif // VERTEX_LAYOUT_HPP

