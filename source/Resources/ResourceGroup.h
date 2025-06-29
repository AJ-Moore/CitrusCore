#pragma once

#include <CCCommon.h>
#include <Resources/ResourceHandle.h>
#include <Resources/AResource.h>

namespace CitrusCore
{
	template <class T>
	class CITRUS_CORE_API ResourceGroup
	{
		static_assert(std::is_base_of<AResourceBase, T>::value, "T must derive from AResource");
	public:
		template<class P>
		ResourcePtr<T> Get(const std::string& path);

	private:

		std::unordered_map<std::string, std::shared_ptr<ResourceHandle<T>>> m_resources;
	};

	template<class T>
	template<typename P>
	inline ResourcePtr<T> ResourceGroup<T>::Get(const std::string& path)
	{
		auto handle = m_resources.find(path);

		if (handle != m_resources.end())
		{
			return handle->second;
		}

		std::shared_ptr<ResourceHandle<T>> resource = std::make_shared<ResourceHandle<T>>(path);
		resource->template Load<P>();
		m_resources[path] = resource;
		return resource;
	}
}