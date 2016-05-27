#ifndef VERTEX_ARRAY_HPP
#define VERTEX_ARRAY_HPP
#include "Prerequisites.hpp"
#include "GpuResource.hpp"

class VertexLayout;
class Renderer;
template <typename T>
class VertexBuffer;

class VertexArray : public GpuResource
{
public:
	VertexArray();

	// Copy/Move constructors and assignments
	VertexArray(const VertexArray &lhs) = delete;
	VertexArray(VertexArray &&rhs);
	VertexArray& operator=(VertexArray cas) = delete;
	VertexArray& operator=(VertexArray &&rhs);
	
	template <typename T>
	VertexArray(VertexBuffer<T> &buffer, Renderer &renderer);
	template <typename T>
	void create(VertexBuffer<T> &buffer, Renderer &renderer);

	~VertexArray();
	void clear();

private:
	
};

template<typename T>
inline VertexArray::VertexArray(VertexBuffer<T> &buffer, Renderer &renderer)
{
	create(buffer, renderer);
}

template<typename T>
inline void VertexArray::create(VertexBuffer<T> &buffer, Renderer &renderer)
{
	static_assert(std::is_base_of<VertexLayout, T>(), "VertexArray must operate on VertexLayout");

	gl::GenVertexArrays(1, &mID);

	renderer.bindVertexArray(*this);
	renderer.bindVertexBuffer(buffer);

	std::uint32_t i = 0;
	for (auto it : T::GetAttrib())
	{
		gl::EnableVertexAttribArray(i);
		gl::VertexAttribPointer(i, it.size, it.type, it.normalized, it.stride,
			reinterpret_cast<void *>(it.pointer));

		i++;
	}
}

#endif // VERTEX_ARRAY_HPP

