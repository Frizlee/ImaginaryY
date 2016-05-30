#ifndef GPU_BUFFER_HPP
#define GPU_BUFFER_HPP
#include "Prerequisites.hpp"
#include "GpuObject.hpp"

enum class GpuBufferType
{
	VERTEX_BUFFER,
	COPY_WRITE_BUFFER,
	COPY_READ_BUFFER
};

class GpuBuffer : public GpuObject
{
public:
	GpuBuffer() = delete;
	GpuBuffer(GpuBufferType type);

	GpuBuffer(const GpuBuffer &lhs) = delete;
	GpuBuffer(GpuBuffer &&rhs);
	GpuBuffer& operator=(const GpuBuffer &lhs) = delete;
	GpuBuffer& operator=(GpuBuffer &&rhs);

	virtual ~GpuBuffer() = 0;
	virtual void clear() = 0;

	GpuBufferType getType() const;

private:
	GpuBufferType mType;
	
};

#endif // GPU_BUFFER_HPP

