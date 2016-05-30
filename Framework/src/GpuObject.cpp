#include "../include/GpuObject.hpp"
using namespace std;

GpuObject::GpuObject()
	: mSize{ 0 }
{
}

GpuObject::GpuObject(GpuObject &&rhs)
{
	swap(mID, rhs.mID);
	swap(mSize, rhs.mSize);
}

GpuObject & GpuObject::operator=(GpuObject &&rhs)
{
	swap(mID, rhs.mID);
	swap(mSize, rhs.mSize);
	return *this;
}

GpuObject::~GpuObject()
{
}

uint32_t GpuObject::getSize() const
{
	return mSize;
}

GLuint GpuObject::getID() const
{
	return mID;
}

