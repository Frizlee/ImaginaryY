#include "../include/ResourceManager.hpp"
#include "../include/DdsCodec.hpp"
#include "../include/PngCodec.hpp"
#include <fstream>
using namespace std;


ResourceBase::~ResourceBase()
{
	if (mRefCounter != 0);// TODO: Error handling, resource is still used.
	
	if (mPtr == nullptr); // TODO: Error handling, resourece was already been deleted.	
}

ResourceBase::ResourceBase(std::string name)
	: mPtr{ nullptr }
{
}

void ResourceBase::addRef()
{
	mRefCounter++;
}

void ResourceBase::subRef()
{
	mRefCounter--;
}


ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
	mResources.clear();
}

void ResourceManager::RemoveUnused()
{
	std::unordered_map<Guid, ResourceBase>::iterator it;
	for (it = mResources.begin(); it != mResources.end();)
	{
		if (it->second.mRefCounter == 0)
			it = mResources.erase(it);
		else
			++it;
	}
}

