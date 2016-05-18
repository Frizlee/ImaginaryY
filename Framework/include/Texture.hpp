#ifndef TEXTURE_HPP
#define TEXTURE_HPP
#include "gl_core_3_3.hpp"
#include "Image.hpp"

class Texture
{
	friend class Renderer;

public:
	Texture();

	Texture(const Image &img);
	void create(const Image &img);

	~Texture();

private:
	GLuint mID;

	void createFromCompressedImage(const Image &img);
};

#endif // TEXTURE_HPP
