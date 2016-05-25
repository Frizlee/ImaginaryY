#ifndef TEXTURE_HPP
#define TEXTURE_HPP
#include "Prerequisites.hpp"
#include "GpuResource.hpp"
#include "Image.hpp"

class Texture : public GpuResource
{
	friend class Renderer;

	enum class Type
	{
		UNCOMPRESSED_2D,
		COMPRESSED_2D,
		UNCOMPRESSED_2D_ARRAY,
		COMPRESSED_2D_ARRAY
	};

public:
	Texture();

	// create texture
	Texture(const Image &img, Renderer &renderer);
	void create(const Image &img, Renderer &renderer);

	// create texture array
	// Must be pointers due to lack of other posibilities
	Texture(std::vector<Image*> &&imgs, Renderer &renderer);
	void create(std::vector<Image*> &&imgs, Renderer &renderer);
	
	~Texture();
	void clear();

	Image::Format getFormat();
	Type getType();

private:
	Image::Format mFormat;
	Type mType;

	static void ParseFormat(Image::Format imgFormat, GLint &format, 
		GLint &internalFormat, GLenum &dataType, GLenum &compressedInternalFormat);
};

#endif // TEXTURE_HPP
