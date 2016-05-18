#include "../include/Texture.hpp"

Texture::Texture()
{
}

Texture::Texture(const Image &img)
{
	create(img);
}

void Texture::create(const Image & img)
{
	GLint format;
	GLint internalFormat;
	GLint dataType;

	gl::GenTextures(1, &mID);

	switch (img.getFormat())
	{
	case Image::Format::R8:
		format = gl::RED;
		internalFormat = gl::R8;
		dataType = gl::UNSIGNED_BYTE;
		break;

	case Image::Format::RG8:
		format = gl::RG;
		internalFormat = gl::RG8;
		dataType = gl::UNSIGNED_BYTE;
		break;

	case Image::Format::RGB8:
		format = gl::RGB;
		internalFormat = gl::RGB8;
		dataType = gl::UNSIGNED_BYTE;
		break;

	case Image::Format::RGBA8:
		format = gl::RGBA;
		internalFormat = gl::RGBA8;
		dataType = gl::UNSIGNED_BYTE;
		break;

	case Image::Format::R3G3B2:
		format = gl::RGB;
		internalFormat = gl::R3_G3_B2;
		dataType = gl::UNSIGNED_BYTE_2_3_3_REV;
		break;

	case Image::Format::R5G6B5:
		format = gl::RGB;
		internalFormat = gl::RGB8;
		dataType = gl::UNSIGNED_SHORT_5_6_5_REV;
		break;

	case Image::Format::RGBA4:
		format = gl::RGBA;
		internalFormat = gl::RGBA4;
		dataType = gl::UNSIGNED_SHORT_4_4_4_4_REV;
		break;

	case Image::Format::RGB5A1:
		format = gl::RGBA;
		internalFormat = gl::RGB5_A1;
		dataType = gl::UNSIGNED_SHORT_1_5_5_5_REV;
		break;

	case Image::Format::RGB10A2:
		format = gl::RGBA;
		internalFormat = gl::RGB10_A2;
		dataType = gl::UNSIGNED_INT_2_10_10_10_REV;
		break;

	case Image::Format::SRGB8:
		format = gl::RGB;
		internalFormat = gl::SRGB8;
		dataType = gl::UNSIGNED_BYTE;
		break;

	case Image::Format::SRGB8A8:
		format = gl::RGBA;
		internalFormat = gl::SRGB8_ALPHA8;
		dataType = gl::UNSIGNED_BYTE;
		break;

	default:
		return createFromCompressedImage(img);
	}


	// TODO: Move binding from here.
	gl::BindTexture(gl::TEXTURE_2D, mID);
	gl::TexImage2D(gl::TEXTURE_2D, 0, internalFormat, img.getWidth(), img.getHeight(),
		0, format, dataType, img.mBytes.data());
	gl::BindTexture(gl::TEXTURE_2D, 0);
}

Texture::~Texture()
{
}

void Texture::createFromCompressedImage(const Image &img)
{
	GLenum internalFormat;

	bool noS3TCExt = !gl::exts::var_EXT_texture_compression_s3tc;

	switch (img.getFormat())
	{
	case Image::Format::BC1_RGB:
		if (noS3TCExt)
			return;

		internalFormat = gl::COMPRESSED_RGB_S3TC_DXT1_EXT;
		break;

	case Image::Format::BC1_RGBA:
		if (noS3TCExt)
			return;

		internalFormat = gl::COMPRESSED_RGBA_S3TC_DXT1_EXT;
		break;

	case Image::Format::BC2_RGBA:
		if (noS3TCExt)
			return;

		internalFormat = gl::COMPRESSED_RGBA_S3TC_DXT3_EXT;
		break;

	case Image::Format::BC3_RGBA:
		if (noS3TCExt)
			return;

		internalFormat = gl::COMPRESSED_RGBA_S3TC_DXT5_EXT;
		break;

	case Image::Format::BC4_R:
		internalFormat = gl::COMPRESSED_RED_RGTC1;
		break;

	case Image::Format::BC4_SIGNED_R:
		internalFormat = gl::COMPRESSED_SIGNED_RED_RGTC1;
		break;

	case Image::Format::BC5_RG:
		internalFormat = gl::COMPRESSED_RG_RGTC2;
		break;

	case Image::Format::BC5_SIGNED_RG:
		internalFormat = gl::COMPRESSED_SIGNED_RG_RGTC2;
		break;

	default:
		// TODO: Error handling.
		return;
	}

	// TODO: Move binding from here.
	gl::BindTexture(gl::TEXTURE_2D, mID);
	gl::CompressedTexImage2D(gl::TEXTURE_2D, 0, internalFormat, img.getWidth(),
		img.getHeight(), 0, img.mBytes.size(), img.mBytes.data());
	gl::BindTexture(gl::TEXTURE_2D, 0);
}
