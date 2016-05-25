#include "../include/Text.hpp"
#include <glm/vec2.hpp>
using namespace std;

Text::Text()
{
}

Text::Text(const string &content, Font &fnt)
{
	create(content, fnt);
}

void Text::create(const string &content, Font &fnt)
{
	mBuffer.clear();

	basic_string<uint32_t> text32 = wstring_convert<
		codecvt_utf8<uint32_t>, uint32_t>{}.from_bytes(content);
	basic_string<uint32_t>::iterator it;

	mBuffer.reserve(text32.length() * 6);

	glm::vec2 penPos(0.0f, 0.0f);

	for (it = begin(text32); it != end(text32); it++)
		fnt.putChar(*it, mBuffer, penPos);
}

const std::vector<TextVertex>& Text::getBuffer()
{
	return mBuffer;
}

