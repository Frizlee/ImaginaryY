#ifndef RENDERER_HPP
#define RENDERER_HPP
#include "gl_core_3_3.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include <cstdint>

class Renderer
{
public:
	Renderer();
	~Renderer();

	void init();

	void bindShader(Shader &shr);
	void bindTexture(Texture &tex, std::int32_t slot);
	void setDepthTest(bool state);

	bool isInitialized() const;
	
private:
	bool mInitialized;
};


#endif // RENDERER_HPP

