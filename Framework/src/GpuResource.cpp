#include "../include/GpuResource.hpp"
using namespace std;

GpuResource::GpuResource()
	: mSize{ 0 }
{
}

GpuResource::GpuResource(GpuResource &&rhs)
{
	swap(mID, rhs.mID);
	swap(mSize, rhs.mSize);
}

GpuResource & GpuResource::operator=(GpuResource && rhs)
{
	swap(mID, rhs.mID);
	swap(mSize, rhs.mSize);
	return *this;
}

GpuResource::~GpuResource()
{
}

uint32_t GpuResource::getSize() const
{
	return mSize;
}

GLuint GpuResource::getID()
{
	return mID;
}

