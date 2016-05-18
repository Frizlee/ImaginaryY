#ifndef RESOURCE_HPP
#define RESOURCE_HPP
#include <string>
#include <cstdint>

class Resource
{
public:
	Resource();
	virtual ~Resource() = 0;

	virtual std::uint32_t getSize() const = 0;

protected:
};


#endif // RESOURCE_HPP

