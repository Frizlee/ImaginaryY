#include "..\include\GpuBuffer.hpp"
using namespace std;

GpuBuffer::GpuBuffer(GpuBufferType type)
	: mType{ type }
{
}

GpuBuffer::GpuBuffer(GpuBuffer &&rhs)
	: GpuObject(move(rhs))
{
	swap(mType, rhs.mType);
}

GpuBuffer& GpuBuffer::operator=(GpuBuffer &&rhs)
{
	GpuObject::operator=(move(rhs));
	swap(mType, rhs.mType);
	return *this;
}

GpuBuffer::~GpuBuffer()
{
}

GpuBufferType GpuBuffer::getType() const
{
	return mType;
}

