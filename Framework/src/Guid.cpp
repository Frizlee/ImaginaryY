#include "../include/Guid.hpp"
#include <Windows.h>
using namespace std;

Guid::Guid()
{
	generateNew();
}

Guid::Guid(const Guid &g)
{
	mData1 = g.mData1;
	mData2 = g.mData2;
	mData3 = g.mData3;
	mData4[0] = g.mData4[0];
	mData4[1] = g.mData4[1];
	mData4[2] = g.mData4[2];
	mData4[3] = g.mData4[3];
	mData4[4] = g.mData4[4];
	mData4[5] = g.mData4[5];
	mData4[6] = g.mData4[6];
	mData4[7] = g.mData4[7];
}

Guid::~Guid()
{
}

void Guid::generateNew()
{
	UUID winUuid;
	UuidCreate(&winUuid);
	mData1 = winUuid.Data1;
	mData2 = winUuid.Data2;
	mData3 = winUuid.Data3;
	mData4[0] = winUuid.Data4[0];
	mData4[1] = winUuid.Data4[1];
	mData4[2] = winUuid.Data4[2];
	mData4[3] = winUuid.Data4[3];
	mData4[4] = winUuid.Data4[4];
	mData4[5] = winUuid.Data4[5];
	mData4[6] = winUuid.Data4[6];
	mData4[7] = winUuid.Data4[7];
}

std::string Guid::toString() const
{
	stringstream str;
	str << hex << setfill('0')
		<< "{" << setw(8) << mData1 << "-"
		<< setw(4) << mData2 << "-"
		<< setw(4) << mData3 << "-"
		<< setw(2) << +mData4[0] << setw(2) << +mData4[1] << "-"
		<< setw(2) << +mData4[2] << setw(2) << +mData4[3]
		<< setw(2) << +mData4[4] << setw(2) << +mData4[5]
		<< setw(2) << +mData4[6] << setw(2) << +mData4[7] << "}";

	return str.str();
}

bool operator==(const Guid &g1, const Guid &g2)
{
	return g1.mData1 == g2.mData1 && g1.mData2 == g2.mData2 &&
		g1.mData3 == g2.mData3 && g1.mData4[0] == g2.mData4[0] &&
		g1.mData4[1] == g2.mData4[1] && g1.mData4[2] == g2.mData4[2] &&
		g1.mData4[3] == g2.mData4[3] && g1.mData4[4] == g2.mData4[4] &&
		g1.mData4[5] == g2.mData4[5] && g1.mData4[6] == g2.mData4[6] &&
		g1.mData4[7] == g2.mData4[7];
}

bool operator==(const std::string &s, const Guid &g)
{
	return s == g.toString();
}

bool operator==(const Guid &g, const std::string &s)
{
	return s == g.toString();
}

std::ostream & operator<<(std::ostream &stream, const Guid &g)
{
	return stream << g.toString();
}


