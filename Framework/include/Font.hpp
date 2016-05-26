#ifndef FONT_HPP
#define FONT_HPP
#include "Prerequisites.hpp"
#include "Resource.hpp"
#include <ft2build.h>
#include FT_FREETYPE_H
#include "ResourceManager.hpp"
#include "ImageAtlas.hpp"
#include "VertexLayout.hpp"

class Font : public Resource
{
	friend class Text;

public:
	Font();
	Font(const std::string &facePath, std::uint8_t size, ResourceHandle<ImageAtlas> atlas, 
		float spacing = 1.5f);
	void create(const std::string &facePath, std::uint8_t size, ResourceHandle<ImageAtlas> atlas, 
		float spacing = 1.5f);
	

	~Font();
	void clear();

	ResourceHandle<ImageAtlas> getAtlas();
	std::uint32_t getSize() const;

private:
	struct GlyphInfo
	{
		float ax, ay;
		float bw, bh;
		float bl, bt;
		uint32_t tx, ty;
	};

	FT_Library FTLib; // TODO: Make it thread local
	FT_Face mFace;
	ResourceHandle<ImageAtlas> mAtlas;

	std::uint8_t mSize;
	std::uint32_t mTabWidth;
	std::int32_t mAscender;
	std::int32_t mDescender;
	std::uint16_t mHeight;
	float mSpacing;

	std::unordered_map<std::uint32_t, GlyphInfo> mChars;

	bool cacheChar(std::uint32_t charCode);
	void putChar(std::uint32_t charCode, std::vector<TextVertexLayout::Data> &buffer, 
		glm::vec2 &penPos);
};

#endif // FONT_HPP

