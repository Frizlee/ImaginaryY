#include "../include/Renderer.hpp"
#include "../include/Shader.hpp"
#include "../include/Texture.hpp"
#include "../include/VertexArray.hpp"
#include "../include/VertexBuffer.hpp"
using namespace std;

Renderer::Renderer() 
	: mInitialized{ false }
{
}

Renderer::~Renderer()
{
}

void Renderer::init()
{
	if (gl::sys::LoadFunctions())
		mInitialized = true;
}

void Renderer::bindShader(Shader &shr)
{
	gl::UseProgram(shr.getID());
}

void Renderer::bindTexture(Texture &tex, int32_t slot)
{
	GLenum target;
	
	switch (tex.getType())
	{
	case Texture::Type::COMPRESSED_2D:
	case Texture::Type::UNCOMPRESSED_2D:
		target = gl::TEXTURE_2D;
		break;

	case Texture::Type::COMPRESSED_2D_ARRAY:
	case Texture::Type::UNCOMPRESSED_2D_ARRAY:
		target = gl::TEXTURE_2D_ARRAY;
		break;
	}

	gl::ActiveTexture(gl::TEXTURE0 + slot);
	gl::BindTexture(target, tex.getID());
}

void Renderer::bindVertexArray(VertexArray &arr)
{
	gl::BindVertexArray(arr.getID());
}

void Renderer::bindBuffer(GpuBuffer &buf)
{
	GpuBufferType t = buf.getType();
	bindBuffer(buf, t);
}

void Renderer::bindBuffer(GpuBuffer &buf, GpuBufferType &type)
{
	GLenum target;

	switch (type)
	{
	case GpuBufferType::VERTEX_BUFFER:
		target = gl::ARRAY_BUFFER;
		break;

	case GpuBufferType::COPY_READ_BUFFER:
		target = gl::COPY_READ_BUFFER;
		break;

	case GpuBufferType::COPY_WRITE_BUFFER:
		target = gl::COPY_WRITE_BUFFER;
		break;

	default:
		return;
	}

	gl::BindBuffer(target, buf.getID());
}

void Renderer::setDepthTest(bool state)
{
	if (state)
		gl::Enable(gl::DEPTH_TEST);
	else
		gl::Disable(gl::DEPTH_TEST);
}

bool Renderer::isInitialized() const
{
	return mInitialized;
}

