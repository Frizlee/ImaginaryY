#ifndef TEXTURE_HPP
#define TEXTURE_HPP
#include "Prerequisites.hpp"
#include "GpuObject.hpp"
#include "Image.hpp"

class Texture : public GpuObject
{
public:
	enum class Type
	{
		UNCOMPRESSED_2D,
		COMPRESSED_2D,
		UNCOMPRESSED_2D_ARRAY,
		COMPRESSED_2D_ARRAY
	};

	Texture();

	// Copy/Move constructors and assignments
	Texture(const Texture &lhs) = delete;
	Texture(Texture &&rhs);
	Texture& operator=(const Texture &lhs) = delete;
	Texture& operator=(Texture &&rhs);

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
