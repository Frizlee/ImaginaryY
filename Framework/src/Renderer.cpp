#include "../include/Renderer.hpp"

Renderer::Renderer() :
	mInitialized{ false }
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
	gl::UseProgram(shr.mID);
}

void Renderer::bindTexture(Texture &tex, int32_t slot)
{
	gl::ActiveTexture(gl::TEXTURE0 + slot);
	gl::BindTexture(gl::TEXTURE_2D, tex.mID);
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

