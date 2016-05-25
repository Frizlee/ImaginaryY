#ifndef GPU_RESOURCE_HPP
#define GPU_RESOURCE_HPP
#include "Prerequisites.hpp"
#include "Resource.hpp"
#include "gl_core_3_3.hpp"

class GpuResource : public Resource
{
public:	
	GpuResource();
	
	virtual ~GpuResource() = 0;
	virtual void clear() = 0;

	std::uint32_t getSize() const;

protected:
	GLuint mID;
	std::uint32_t mSize;

private:
	GLuint getID();
};

#endif // GPU_RESOURCE_HPP

