#ifndef GUID_GENERATOR_HPP
#define GUID_GENERATOR_HPP
#include "Prerequisites.hpp"

class Guid
{
public:
	Guid();
	Guid(const Guid &g);
	~Guid();

	void generateNew();
	
	std::string toString() const;

	friend bool operator==(const Guid&, const Guid&);
	friend bool operator==(const std::string&, const Guid&);
	friend bool operator==(const Guid&, const std::string&);
	friend std::ostream& operator<<(std::ostream&, const Guid&);
	friend struct std::hash<Guid>;

private:
	std::uint32_t mData1;
	std::uint16_t mData2;
	std::uint16_t mData3;
	std::uint8_t mData4[8];
	
};

namespace std
{
	template <>
	struct hash<Guid>
	{
		std::size_t operator()(const Guid &g) const
		{
			std::size_t output = 0;
			output ^= g.mData1;
			output ^= g.mData2;
			output ^= g.mData3;
			output ^= g.mData4[0];
			output ^= g.mData4[1];
			output ^= g.mData4[2];
			output ^= g.mData4[3];
			output ^= g.mData4[4];
			output ^= g.mData4[5];
			output ^= g.mData4[6];
			output ^= g.mData4[7];

			return output;
		}

	};
}

#endif // GUID_GENERATOR_HPP

