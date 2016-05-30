#include "../include/VertexArray.hpp"
#include "../include/VertexLayout.hpp"
#include "../include/GpuBuffer.hpp"
#include "../include/Renderer.hpp"
using namespace std;

VertexArray::VertexArray()
{
}

VertexArray::VertexArray(VertexArray &&rhs)
	: GpuObject(move(rhs))
{
}

VertexArray& VertexArray::operator=(VertexArray &&rhs)
{
	GpuObject::operator=(move(rhs));

	return *this;
}

VertexArray::~VertexArray()
{
	gl::DeleteVertexArrays(1, &mID);
}

void VertexArray::clear()
{
}

