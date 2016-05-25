#ifndef TEXT_HPP
#define TEXT_HPP
#include "Prerequisites.hpp"
#include "Font.hpp"
#include "VertexLayouts.hpp"

class Text
{
public:
	Text();
	Text(const std::string &content, Font &fnt);
	void create(const std::string &content, Font &fnt);
	
	const std::vector<TextVertex>& getBuffer();

private:
	std::vector<TextVertex> mBuffer;
};

#endif // MESH_HPP

