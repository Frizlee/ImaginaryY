#include "../include/DdsCodec.hpp"
#include "../include/Image.hpp"
using namespace std;

static const uint32_t DDS_MAGIC = 0x20534444; // (DDS )
static const uint32_t DDS_BC1 = 0x31545844; // (DXT1)
static const uint32_t DDS_BC2 = 0x33545844; // (DXT3)
static const uint32_t DDS_BC3 = 0x35545844; // (DXT5)
static const uint32_t DDS_BC4U = 0x55344342; // (BC4U)
static const uint32_t DDS_BC4S = 0x53344342; // (BC4S)
static const uint32_t DDS_BC5U = 0x32495441; // (ATI2)
static const uint32_t DDS_BC5S = 0x53354342; // (BC5S)
static const uint32_t DDS_DX10 = 0x30315844; // (DX10)

enum DDSFlags
{
	DDPF_ALPHAPIXELS	= 0x1,
	DDPF_ALPHA			= 0x2,
	DDPF_FOURCC			= 0x4,
	DDPF_RGB			= 0x40,
	DDPF_YUV			= 0x200,
	DDPF_LUMINANCE		= 0x20000,

	DDSD_CAPS			= 0x1,
	DDSD_HEIGHT			= 0x2,
	DDSD_WIDTH			= 0x4,
	DDSD_PITCH			= 0x8,
	DDSD_PIXELFORMAT	= 0x1000,
	DDSD_MIPMAPCOUNT	= 0x20000,
	DDSD_LINEARSIZE		= 0x80000,
	DDSD_DEPTH			= 0x800000,

	DDSCAPS_COMPLEX		= 0x8, 
	DDSCAPS_MIPMAP		= 0x400000,
	DDSCAPS_TEXTURE		= 0x1000,

	DDSCAPS2_CUBEMAP			= 0x200,
	DDSCAPS2_CUBEMAP_POSITIVEX	= 0x400,
	DDSCAPS2_CUBEMAP_NEGATIVEX	= 0x800,
	DDSCAPS2_CUBEMAP_POSITIVEY	= 0x1000,
	DDSCAPS2_CUBEMAP_NEGATIVEY	= 0x2000,
	DDSCAPS2_CUBEMAP_POSITIVEZ	= 0x4000,
	DDSCAPS2_CUBEMAP_NEGATIVEZ	= 0x8000,
	DDSCAPS2_VOLUME				= 0x200000,
};

struct RGBAMask
{
	uint32_t r;
	uint32_t g;
	uint32_t b;
	uint32_t a;
	
	RGBAMask(uint32_t r, uint32_t g, uint32_t b, uint32_t a) 
		: r{r}, g{g}, b{b}, a{a} {}
	RGBAMask(const uint32_t *bytes)
	{
		r = bytes[0];
		g = bytes[1];
		b = bytes[2];
		a = bytes[3];
	}
	
	bool operator==(const RGBAMask &m)
	{
		return (r == m.r && g == m.g && b == m.b && a == m.a);
	}
};

static const RGBAMask BGR8mask		{ 0x00ff0000, 0x0000ff00, 0x000000ff, 0x00000000 };
static const RGBAMask BGRA8mask		{ 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000 };
static const RGBAMask B2G3R3mask	{ 0x000000e0, 0x0000001c, 0x00000003, 0x00000000 };
static const RGBAMask B5G6R5mask	{ 0x0000f800, 0x000007e0, 0x0000001f, 0x00000000 };
static const RGBAMask BGRA4mask		{ 0x00000f00, 0x000000f0, 0x0000000f, 0x0000f000 };
static const RGBAMask BGR5A1mask	{ 0x00007c00, 0x000003e0, 0x0000001f, 0x00008000 };
static const RGBAMask BGR10A2mask	{ 0x3ff00000, 0x000ffc00, 0x000003ff, 0xc0000000 };

#pragma pack(push, 1)

struct DdsPixelFormat
{
	uint32_t dwSize;
	uint32_t dwFlags;
	uint32_t dwFourCC;
	uint32_t dwRGBBitCount;
	uint32_t dwRBitMask;
	uint32_t dwGBitMask;
	uint32_t dwBBitMask;
	uint32_t dwABitMask;
};

struct DdsHeader
{
	uint32_t dwMagic;
	uint32_t dwSize;
	uint32_t dwFlags;
	uint32_t dwHeight;
	uint32_t dwWidth;
	uint32_t dwPitchOrLinearSize;
	uint32_t dwDepth;
	uint32_t dwMipmapCount;
	uint32_t dwReserved1[11];
	DdsPixelFormat pixelFormat;
	uint32_t dwCaps;
	uint32_t dwCaps2;
	uint32_t dwCaps3;
	uint32_t dwCaps4;
	uint32_t dwReserved2;
};

#pragma pack(pop)



void DdsCodec::BC1Block::flipVerticaly()
{
	uint8_t temp8;

	temp8 = ponm;
	ponm = dcba;
	dcba = temp8;

	temp8 = lkji;
	lkji = hgfe;
	hgfe = temp8;
}

void DdsCodec::BC4Block::flipVerticaly()
{
	uint32_t temp32;
	uint32_t *as32[2];

	as32[0] = reinterpret_cast<uint32_t*>(ahgfedcba);
	as32[1] = reinterpret_cast<uint32_t*>(aponmlkji);

	temp32 = *as32[0] & ((1 << 12) - 1);
	*as32[0] &= ~((1 << 12) - 1);
	*as32[0] |= (*as32[1] & (((1 << 12) - 1) << 12)) >> 12;
	*as32[1] &= ~(((1 << 12) - 1) << 12);
	*as32[1] |= temp32 << 12;

	temp32 = *as32[0] & (((1 << 12) - 1) << 12);
	*as32[0] &= ~(((1 << 12) - 1) << 12);
	*as32[0] |= (*as32[1] & ((1 << 12) - 1)) << 12;
	*as32[1] &= ~((1 << 12) - 1);
	*as32[1] |= temp32 >> 12;
}

void DdsCodec::BC2Block::flipVerticaly()
{
	uint16_t temp16;

	temp16 = aponm;
	aponm = adcab;
	adcab = temp16;

	temp16 = alkji;
	alkji = ahgfe;
	ahgfe = temp16;

	c.flipVerticaly();
}

void DdsCodec::BC3Block::flipVerticaly()
{
	a.flipVerticaly();
	c.flipVerticaly();
}

void DdsCodec::BC5Block::flipVerticaly()
{
	r.flipVerticaly();
	a.flipVerticaly();
}

DdsCodec::DdsCodec()
{
}

DdsCodec::~DdsCodec()
{
}

void DdsCodec::decode(std::istream &input, Resource &res)
{
	DdsHeader header;
	DdsPixelFormat *pixelFormat;
	Image *imgRes;

	//if (res->getType() != Resource::Type::IMAGE)
	//{
	//	// TODO: Error handling.
	//	return;
	//}

	imgRes = dynamic_cast<Image*>(&res);
	
	if (!(input.read(reinterpret_cast<char*>(&header), sizeof(header))) || header.dwMagic != DDS_MAGIC)
	{
		// TODO: Error handling.
		return;
	}

	if (!((header.dwFlags & DDSD_CAPS) && (header.dwFlags & DDSD_HEIGHT) &&
		(header.dwFlags & DDSD_WIDTH) && (header.dwFlags & DDSD_PIXELFORMAT)))
	{
		// TODO: Error handling.
		return;
	}

	pixelFormat = &header.pixelFormat;

	if (pixelFormat->dwFlags & DDPF_FOURCC)
	{
		// Compressed.
		Image::Format format;
		size_t blockSize;
		vector<uint8_t> block;
		vector<uint8_t> bytes;
		uint32_t blockWidth;
		uint32_t blockHeight;

		switch (pixelFormat->dwFourCC)
		{
		case DDS_BC1:
			format = Image::Format::BC1_RGB;
			break;

		case DDS_BC2:
			format = Image::Format::BC2_RGBA;
			break;

		case DDS_BC3:
			format = Image::Format::BC3_RGBA;
			break;

		case DDS_BC4S:
			format = Image::Format::BC4_SIGNED_R;
			break;

		case DDS_BC4U:
			format = Image::Format::BC4_R;
			break;

		case DDS_BC5S:
			format = Image::Format::BC5_SIGNED_RG;
			break;

		case DDS_BC5U:
			format = Image::Format::BC5_RG;
			break;

		default:
			// TODO: Error handling.
			return;
		}

		
		blockHeight = (header.dwHeight + 3) / 4;
		blockWidth = (header.dwWidth + 3) / 4;
		blockSize = Image::CalculateBytesPerPixel(format);
		bytes.resize(blockWidth * blockHeight * blockSize);
		block.resize(blockSize);

		for (uint32_t i = 0; i < blockHeight; i++)
		{
			uint32_t row = (blockHeight - i - 1) * blockWidth * blockSize;

			for (uint32_t j = 0; j < blockWidth * blockSize; j += blockSize)
			{
				if (!input.read(reinterpret_cast<char*>(block.data()), blockSize))
				{
					// TODO: Error handling.
					return;
				}

				flip(block, format);
				copy(begin(block), end(block), begin(bytes) + row + j);
			}
		}

		imgRes->create(header.dwWidth, header.dwHeight, format, move(bytes));
	}
	else if (pixelFormat->dwFlags & DDPF_RGB)
	{
		// Uncompressed.
		bool hasAlpha = pixelFormat->dwFlags & DDPF_ALPHAPIXELS;
		Image::Format format;
		vector<uint8_t> bytes;
		uint32_t bytesPerPixel;
		union
		{
			uint8_t pixel[4];
			uint32_t pixel32;
		};
		RGBAMask mask(&pixelFormat->dwRBitMask);
		
		if (hasAlpha == false)
			mask.a = 0;

		if (mask == BGR8mask)
			format = Image::Format::RGB8;
		else if (mask == BGRA8mask)
			format = Image::Format::RGBA8;
		else if (mask == B2G3R3mask)
			format = Image::Format::R3G3B2;
		else if (mask == B5G6R5mask)
			format = Image::Format::R5G6B5;
		else if (mask == BGRA4mask)
			format = Image::Format::RGBA4;
		else if (mask == BGR5A1mask)
			format = Image::Format::RGB5A1;
		else if (mask == BGR10A2mask)
			format = Image::Format::RGB10A2;
		else
		{
			// TODO: Error handling.
			return;
		}

		// imgRes->create(header.dwWidth, header.dwHeight, format);
		bytesPerPixel = Image::CalculateBytesPerPixel(format);
		bytes.resize(header.dwWidth * header.dwHeight * bytesPerPixel);
		
		pixel[0] = 0;
		pixel[1] = 0;
		pixel[2] = 0;
		pixel[3] = 0;

		uint32_t comp[4]; // 0 - red, 1 - green, 2 - blue, 3 - alpha;
		int8_t shifts[3]; // 0 - red, 1 - green, 2 - blue;

		shifts[0] = popcount(mask.g | mask.b);
		shifts[1] = popcount(mask.b) - popcount(mask.r);
		shifts[2] = popcount(mask.r | mask.g);


		for (uint32_t i = 0; i < header.dwHeight; i++)
		{
			uint32_t row = (header.dwHeight - i - 1) * header.dwWidth * bytesPerPixel;

			for (uint32_t j = 0; j < header.dwWidth * bytesPerPixel; j += bytesPerPixel)
			{
				if (!input.read(reinterpret_cast<char*>(&pixel), bytesPerPixel))
				{
					// TODO: Error handling.
					return;
				}

				comp[0] = pixel32 & mask.r;
				comp[1] = pixel32 & mask.g;
				comp[2] = pixel32 & mask.b;
				comp[3] = pixel32 & mask.a;

				pixel32 = comp[0] >> shifts[0] |
					comp[2] << shifts[2] |
					comp[3];

				if (shifts[1] > 0)
					pixel32 |= comp[1] >> shifts[1];
				else if (shifts[1] < 0)
					pixel32 |= comp[1] << -shifts[1];
				else
					pixel32 |= comp[1];

				copy(pixel, pixel + bytesPerPixel, begin(bytes) + row + j);
			}
		}

		imgRes->create(header.dwWidth, header.dwHeight, format, move(bytes));
	}
}

void DdsCodec::encode(std::ostream &output, Resource &res)
{
	return;
}

void DdsCodec::flip(vector<uint8_t> &in, Image::Format format)
{
	switch (format)
	{
	case Image::Format::BC1_RGB:
		reinterpret_cast<BC1Block*>(in.data())->flipVerticaly();
		break;

	case Image::Format::BC2_RGBA:
		reinterpret_cast<BC2Block*>(in.data())->flipVerticaly();
		break;

	case Image::Format::BC3_RGBA:
		reinterpret_cast<BC3Block*>(in.data())->flipVerticaly();
		break;
		
	case Image::Format::BC4_SIGNED_R:
	case Image::Format::BC4_R:
		reinterpret_cast<BC1Block*>(in.data())->flipVerticaly();
		break;

	case Image::Format::BC5_SIGNED_RG:
	case Image::Format::BC5_RG:
		reinterpret_cast<BC5Block*>(in.data())->flipVerticaly();
		break;
	}
}

uint32_t DdsCodec::popcount(uint32_t i)
{
	i = i - ((i >> 1) & 0x55555555);
	i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
	return (((i + (i >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
}


