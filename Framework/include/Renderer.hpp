#ifndef RENDERER_HPP
#define RENDERER_HPP
#include "Prerequisites.hpp"
#include "gl_core_3_3.hpp"

class Shader;
class Texture;
class VertexArray;
class GpuBuffer;
enum class GpuBufferType;

class Renderer
{
public:
	Renderer();
	~Renderer();

	void init();

	void bindShader(Shader &shr);
	void bindTexture(Texture &tex, std::int32_t slot);
	void bindVertexArray(VertexArray &arr);
	void bindBuffer(GpuBuffer &buf);
	void bindBuffer(GpuBuffer &buf, GpuBufferType &type);
	void setDepthTest(bool state);

	bool isInitialized() const;
	
private:
	bool mInitialized;
};

#endif // RENDERER_HPP

