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

	// Copy/Move constructors and assignments
	ResourceHandle(const ResourceHandle<T> &lhs);
	ResourceHandle(ResourceHandle<T> &&rhs);
	ResourceHandle<T>& operator=(ResourceHandle<T> cas);
	ResourceHandle<T>& operator=(ResourceHandle<T> &&rhs);
	
	~ResourceHandle();
	void clear();

	T& operator*() const;
	T* operator->() const;
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
inline ResourceHandle<T>::ResourceHandle(const ResourceHandle<T> &lhs)
{	
	mBase = lhs.mBase;
	mBase->addRef();
}

template<typename T>
inline ResourceHandle<T>::ResourceHandle(ResourceHandle<T> &&rhs)
{
	swap(mBase, rhs.mBase);
}

template<typename T>
inline ResourceHandle<T>& ResourceHandle<T>::operator=(ResourceHandle<T> cas)
{
	// Copy constructor allready called.

	if (mBase != nullptr)
		mBase->subRef();

	swap(mBase, cas.mBase);
	return *this;
}

template<typename T>
inline ResourceHandle<T>& ResourceHandle<T>::operator=(ResourceHandle<T> &&rhs)
{
	swap(mBase, rhs.mBase);
	return *this;
}

template<typename T>
inline ResourceHandle<T>::~ResourceHandle()
{
	if (mBase != nullptr)
		mBase->subRef();
}

template<typename T>
inline void ResourceHandle<T>::clear()
{
	mBase->subRef();
	mBase = nullptr;
}

template<typename T>
inline T& ResourceHandle<T>::operator*() const
{
	return *dynamic_cast<T*>(mBase->mPtr.get());
}

template<typename T>
inline T* ResourceHandle<T>::operator->() const
{
	return dynamic_cast<T*>(mBase->mPtr.get());
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
	static_assert(std::is_base_of<Resource, T>(), "getFreeHandle must operate on Resource");

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

