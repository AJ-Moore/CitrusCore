#pragma once

#include <CCCommon.h>
#include <Resources/ResourceHandle.h>

namespace CitrusCore
{
	template <class T>
	class CITRUS_CORE_API ResourceGroup
	{
		static_assert(std::is_base_of<AResource, T>::value, "T must derive from AResource");
	public:
		std::shared_ptr<ResourceHandle<T>> Get(const std::string& path);

	private:
		void LoadResource(ResourceHandle<T>& resource, const std::string& path);

		std::unordered_map<std::string, std::shared_ptr<ResourceHandle<T>>> m_resources;
	};

	template<class T>
	inline std::shared_ptr<ResourceHandle<T>> ResourceGroup<T>::Get(const std::string& path)
	{
		ResourceHandle<T> handle = m_resources.find(path);

		if (handle != m_resources.end())
		{
			return handle->second;
		}

		std::shared_ptr<ResourceHandle<T>> resource = std::make_shared<ResourceHandle<T>>();
		resource->Load();
		LoadResource(*resource, path);
		m_resources[path] = resource;
		return resource;
	}

	template<class T>
	inline void ResourceGroup<T>::LoadResource(ResourceHandle<T>& resource, const std::string& path)
	{
		resource.SetResourceLocation(path);
		resource.LoadResource(path);
	}
}