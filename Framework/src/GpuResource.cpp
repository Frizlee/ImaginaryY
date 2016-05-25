#include "../include/GpuResource.hpp"

GpuResource::GpuResource()
	: mSize{ 0 }
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

