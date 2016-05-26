#ifndef TEXT_HPP
#define TEXT_HPP
#include "Prerequisites.hpp"
#include "Font.hpp"
#include "VertexLayout.hpp"

class Text
{
public:
	Text();
	Text(const std::string &content, Font &fnt);
	void create(const std::string &content, Font &fnt);
	
	const std::vector<TextVertexLayout::Data>& getBuffer();

private:
	std::vector<TextVertexLayout::Data> mBuffer;
};

#endif // MESH_HPP

