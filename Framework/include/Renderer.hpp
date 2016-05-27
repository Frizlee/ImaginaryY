#ifndef RENDERER_HPP
#define RENDERER_HPP
#include "Prerequisites.hpp"
#include "gl_core_3_3.hpp"

class Shader;
class Texture;
class VertexArray;
template <typename T>
class VertexBuffer;

class Renderer
{
public:
	Renderer();
	~Renderer();

	void init();

	void bindShader(Shader &shr);
	void bindTexture(Texture &tex, std::int32_t slot);
	void bindVertexArray(VertexArray &arr);
	template <typename T>
	void bindVertexBuffer(VertexBuffer<T> &buf);
	void setDepthTest(bool state);

	bool isInitialized() const;
	
private:
	bool mInitialized;
};

template<typename T>
inline void Renderer::bindVertexBuffer(VertexBuffer<T> &buf)
{
	gl::BindBuffer(gl::ARRAY_BUFFER, buf.getID());
}

#endif // RENDERER_HPP

