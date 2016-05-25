#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP
#include "Prerequisites.hpp"
#include "Guid.hpp"
#include "Resource.hpp"
#include "Codec.hpp"


class ResourceBase
{
	template <typename> 
	friend class ResourceHandle;
	friend class ResourceManager;

public:
	~ResourceBase();
	
	ResourceBase(std::string name);

private:	
	void addRef();
	void subRef();

	template <typename T>
	void init();
	
	std::unique_ptr<Resource> mPtr;
	std::string mName;
	std::uint32_t mRefCounter;
};


template <typename T>
class ResourceHandle
{
	friend class ResourceManager;

public:
	ResourceHandle();
	ResourceHandle(const ResourceHandle<T> &r);
	~ResourceHandle();

	ResourceHandle<T>& operator=(const ResourceHandle<T>&);

	T* get();
	std::string getName();

private:
	ResourceHandle(ResourceBase *base);

	ResourceBase *mBase;
};


class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

	template <typename T>
	ResourceHandle<T> getFreeHandle(std::string name);

	void RemoveUnused();
	
private:
	std::unordered_map<Guid, ResourceBase> mResources;
};


template<typename T>
inline void ResourceBase::init()
{
	mPtr = make_unique<T>();
}

template<typename T>
inline ResourceHandle<T>::ResourceHandle()
	: mBase{ nullptr }
{
}

template<typename T>
inline ResourceHandle<T>::ResourceHandle(const ResourceHandle<T> &r)
{	
	mBase = r.mBase;
	mBase->addRef();
}

template<typename T>
inline ResourceHandle<T>::~ResourceHandle()
{
	mBase->subRef();
}

template<typename T>
inline ResourceHandle<T>& ResourceHandle<T>::operator=(const ResourceHandle<T> &other)
{
	if (this != &other)
	{
		if (mBase != nullptr)
		{
			mBase->subRef();
		}

		mBase = other.mBase;
		mBase->addRef();
	}
	return *this;
}

template<typename T>
inline T* ResourceHandle<T>::get()
{
	return dynamic_cast<T*>(mBase->mPtr.get());
}

template<typename T>
inline std::string ResourceHandle<T>::getName()
{
	return mBase->mName;
}

template<typename T>
inline ResourceHandle<T>::ResourceHandle(ResourceBase *base)
{
	mBase = base;
	mBase->addRef();
}

template<typename T>
inline ResourceHandle<T> ResourceManager::getFreeHandle(std::string name)
{
	std::pair<std::unordered_map<Guid, ResourceBase>::iterator, bool> ret;

	do
	{
		ret = mResources.emplace(std::piecewise_construct, 
			std::forward_as_tuple(), std::forward_as_tuple(name));
	} while (ret.second == false);

	ret.first->second.init<T>();

	ResourceHandle<T> handle(&ret.first->second);
	// Guid g = ret.first->first;

	return handle;
}

#endif // RESOURCE_MANAGER_HPP


