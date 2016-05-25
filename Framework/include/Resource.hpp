#ifndef RESOURCE_HPP
#define RESOURCE_HPP
#include "Prerequisites.hpp"

class Resource
{
public:
	Resource();
	
	virtual ~Resource() = 0;
	virtual void clear() = 0;

	virtual std::uint32_t getSize() const = 0;

protected:

private:
};


#endif // RESOURCE_HPP

