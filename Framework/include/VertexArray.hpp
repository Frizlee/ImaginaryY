#ifndef VERTEX_ARRAY_HPP
#define VERTEX_ARRAY_HPP
#include "Prerequisites.hpp"
#include "ResourceManager.hpp"
#include "GpuObject.hpp"

class VertexLayout;
class Renderer;
class GpuBuffer;
template <typename T>
class VertexBuffer;

class VertexArray : public GpuObject
{
public:
	VertexArray();

	// Copy/Move constructors and assignments
	VertexArray(const VertexArray &lhs) = delete;
	VertexArray(VertexArray &&rhs);
	VertexArray& operator=(VertexArray cas) = delete;
	VertexArray& operator=(VertexArray &&rhs);
	
	template <typename T>
	VertexArray(std::shared_ptr<VertexBuffer<T>> buffer, Renderer &renderer);
	template <typename T>
	void create(std::shared_ptr<VertexBuffer<T>> buffer, Renderer &renderer);

	~VertexArray();
	void clear();

private:
	std::shared_ptr<GpuBuffer> mBuffer;
};


template <typename T>
VertexArray::VertexArray(std::shared_ptr<VertexBuffer<T>> buffer, Renderer &renderer)
{
	create(layout, buffer, renderer);
}

template <typename T>
void VertexArray::create(std::shared_ptr<VertexBuffer<T>> buffer, Renderer &renderer)
{
	mBuffer = std::dynamic_pointer_cast<GpuBuffer>(buffer);

	gl::GenVertexArrays(1, &mID);
	T layout;
	renderer.bindVertexArray(*this);
	renderer.bindBuffer(*buffer);
	std::uint32_t i = 0;

	for (auto it : layout.getFormat())
	{
		if (it.empty)
			gl::DisableVertexAttribArray(i);
		else
		{
			gl::EnableVertexAttribArray(i);
			gl::VertexAttribPointer(i, it.size, it.type, it.normalized, it.stride,
				reinterpret_cast<void *>(it.pointer));
		}

		i++;
	}
}

#endif // VERTEX_ARRAY_HPP

