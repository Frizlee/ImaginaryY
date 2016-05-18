#ifndef IMAGE_ATLAS_HPP
#define IMAGE_ATLAS_HPP
#include "Image.hpp"
#include <memory>
#include <glm/vec4.hpp>
#include <cstdint>


class ImageAtlas : public Image
{
public:
	typedef glm::tvec4<std::uint32_t> Rect;

	ImageAtlas();

	// Move constructors and assignments
	// Non copyable
	ImageAtlas(const ImageAtlas& lhs) = delete;
	ImageAtlas(ImageAtlas &&rhs);
	ImageAtlas& operator=(ImageAtlas cas) = delete;
	ImageAtlas& operator=(ImageAtlas &&rhs);

	// Override constructors
	ImageAtlas(std::uint32_t width, std::uint32_t height, Format format, 
		const std::vector<uint8_t> &bytes);
	ImageAtlas(std::uint32_t width, std::uint32_t height, Format format, 
		std::vector<uint8_t> &&bytes);

	~ImageAtlas();

	Rect insert(Image &img);

private:
	struct Node
	{
		std::unique_ptr<Node> mChilds[2];
		Rect mRect;
		bool mFree;

		Node();

		// Move constructors and assignments
		// Non copyable
		Node(const Node &lhs) = delete;
		Node(Node &&rhs);
		//Node& operator=(Node cas) = delete;
		Node& operator=(Node &&rhs);

		Rect insert(std::uint32_t width, std::uint32_t height);
	};

	std::unique_ptr<Node> mRoot;
};

#endif // IMAGE_ATLAS_HPP

