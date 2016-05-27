#ifndef CODEC_HPP
#define CODEC_HPP
#include "Prerequisites.hpp"

class Resource;

// Move it somewhere.
struct memorybuf : std::streambuf
{
	memorybuf(char *base, std::ptrdiff_t size)
	{
		setg(base, base, base + size);
	}
};


class Codec
{
public:
	Codec();
	virtual ~Codec() = 0;

	virtual void decode(std::istream &input, Resource &res) = 0;
	virtual void encode(std::ostream &output, Resource &res) = 0;

private:

};

#endif // CODEC_HPP

