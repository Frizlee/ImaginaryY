#ifndef DDS_CODEC_HPP
#define DDS_CODEC_HPP
#include "Codec.hpp"
#include "Image.hpp"

class DdsCodec : public Codec
{
public:
	#pragma pack(push, 1)

	struct BC1Block
	{
		std::uint16_t c[2];
		std::uint8_t dcba, hgfe, lkji, ponm;

		void flipVerticaly();
	};

	struct BC4Block
	{
		std::uint8_t c[2];
		std::uint8_t ahgfedcba[3];
		std::uint8_t aponmlkji[3];

		void flipVerticaly();
	};

	struct BC2Block
	{
		std::uint16_t adcab, ahgfe, alkji, aponm;
		BC1Block c;

		void flipVerticaly();
	};

	struct BC3Block
	{
		BC4Block a;
		BC1Block c;

		void flipVerticaly();
	};

	struct BC5Block
	{
		BC4Block r;
		BC4Block a;

		void flipVerticaly();
	};

	#pragma pack(pop)

	DdsCodec();
	~DdsCodec();

	void decode(std::istream &input, Resource &res);
	void encode(std::ostream &output, Resource &res);

private:
	void flip(std::vector<std::uint8_t> &in, Image::Format format);
	std::uint32_t popcount(std::uint32_t i);
};				 
				 
#endif // DDS_CODEC_HPP

