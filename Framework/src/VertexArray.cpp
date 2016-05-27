#include "..\include\VertexArray.hpp"
using namespace std;

VertexArray::VertexArray()
{
}

VertexArray::VertexArray(VertexArray &&rhs)
	: GpuResource(move(rhs))
{
}

VertexArray& VertexArray::operator=(VertexArray &&rhs)
{
	GpuResource::operator=(move(rhs));

	return *this;
}

VertexArray::~VertexArray()
{
	gl::DeleteVertexArrays(1, &mID);
}

void VertexArray::clear()
{
}

