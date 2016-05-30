#ifndef VERTEX_BUFFER_HPP
#define VERTEX_BUFFER_HPP
#include "Prerequisites.hpp"
#include "GpuBuffer.hpp"
#include "VertexLayout.hpp"
#include "Renderer.hpp"

template <typename T>
class VertexBuffer : public GpuBuffer
{
public:
	typedef T Layout;

	VertexBuffer();

	VertexBuffer<T>(const VertexBuffer<T> &lhs) = delete;
	VertexBuffer<T>(VertexBuffer<T> &&rhs);
	VertexBuffer<T>& operator=(VertexBuffer<T> cas) = delete;
	VertexBuffer<T>& operator=(VertexBuffer<T> &&rhs);

	VertexBuffer(std::uint32_t size, Renderer &renderer);
	void create(std::uint32_t size, Renderer &renderer);

	~VertexBuffer();
	void clear();

	std::uint32_t add(const std::vector<typename T::Data> &lhsData, Renderer &renderer);
	std::uint32_t add(std::vector<typename T::Data> &&rhsData, Renderer &renderer);

private:
	std::uint32_t mPosition;
};

template<typename T>
inline VertexBuffer<T>::VertexBuffer()
	: GpuBuffer(GpuBufferType::VERTEX_BUFFER), mPosition{ 0 }
{
	static_assert(std::is_base_of<VertexLayout, T>(), "VertexBuffer must operate on VertexLayout");
}

template<typename T>
inline VertexBuffer<T>::VertexBuffer(VertexBuffer<T> &&rhs)
	: GpuBuffer(move(rhs))
{
	swap(mPosition, rhs.mPosition);
}

template<typename T>
inline VertexBuffer<T>& VertexBuffer<T>::operator=(VertexBuffer<T> &&rhs)
{
	GpuBuffer::operator=(move(rhs));
	swap(mPosition, rhs.mPosition);
	return *this;
}

template<typename T>
inline VertexBuffer<T>::VertexBuffer(std::uint32_t size, Renderer &renderer)
	: GpuBuffer(GpuBufferType::VERTEX_BUFFER), mPosition{ 0 }
{
	static_assert(std::is_base_of<VertexLayout, T>(), "VertexBuffer must operate on VertexLayout");
	create(size, renderer);
}

template<typename T>
inline void VertexBuffer<T>::create(std::uint32_t size, Renderer &renderer)
{
	gl::GenBuffers(1, &mID);
	renderer.bindBuffer(*this);
	gl::BufferData(gl::ARRAY_BUFFER, size * T::Size(), nullptr, gl::DYNAMIC_DRAW);
}

template<typename T>
inline VertexBuffer<T>::~VertexBuffer()
{
	gl::DeleteBuffers(1, &mID);
}

template<typename T>
inline void VertexBuffer<T>::clear()
{
	mPosition = 0;
}

template<typename T>
inline std::uint32_t VertexBuffer<T>::add(const std::vector<typename T::Data> &lhsData, Renderer &renderer)
{
	std::uint32_t currentPos = mPosition;

	renderer.bindBuffer(*this);
	gl::BufferSubData(gl::ARRAY_BUFFER, mPosition, lhsData.size() * T::Size(), lhsData.data());
	mPosition += lhsData.size() * T::Size();

	return currentPos;
}

template<typename T>
inline std::uint32_t VertexBuffer<T>::add(std::vector<typename T::Data> &&rhsData, Renderer &renderer)
{
	std::uint32_t currentPos = mPosition;

	renderer.bindBuffer(*this);
	gl::BufferSubData(gl::ARRAY_BUFFER, mPosition, rhsData.size() * T::Size(), rhsData.data());
	mPosition += rhsData.size() * T::Size();

	return currentPos;
}

#endif // VERTEX_BUFFER_HPP

