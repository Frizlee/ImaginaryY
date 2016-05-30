#ifndef GPU_RESOURCE_HPP
#define GPU_RESOURCE_HPP
#include "Prerequisites.hpp"
#include "gl_core_3_3.hpp"

class GpuObject
{
	friend class Renderer;

public:	
	GpuObject();
	
	GpuObject(const GpuObject &lhs) = delete;
	GpuObject(GpuObject &&rhs);
	GpuObject& operator=(const GpuObject &lhs) = delete;
	GpuObject& operator=(GpuObject &&rhs);

	virtual ~GpuObject() = 0;
	virtual void clear() = 0;

	std::uint32_t getSize() const;

protected:
	GLuint mID;
	std::uint32_t mSize;

private:
	GLuint getID() const;
};

#endif // GPU_RESOURCE_HPP

