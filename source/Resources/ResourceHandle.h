#pragma once

#include <Resources/AResource.h>
#include <CCCommon.h>

namespace CitrusCore
{
	template<class T> 
	class ResourceGroup;

	template <class T>
	class CITRUS_CORE_API ResourceHandle
	{
		static_assert(std::is_base_of<AResource<T>, T>::value, 
			"T must derive from AResource");
		friend class ResourceGroup<T>;
	public:
		ResourceHandle() = delete;
		ResourceHandle(std::string path) { m_path = path; }
		T* GetResource() { return m_resource.get(); }
	protected:
		virtual void Load(); 
		virtual void Unload();
	private: 
		std::string m_path;
		std::unique_ptr<T> m_resource = nullptr;
		bool m_bLoaded = false;
	};

	template <class T>
	using ResourcePtr = std::shared_ptr<ResourceHandle<T>>;

	template<class T>
	void ResourceHandle<T>::Load() 
	{
		AResource<T>* resource = (AResource<T>*)GetResource();
		resource->LoadResource(m_path);
	}

	template<class T>
	void ResourceHandle<T>::Unload() 
	{
		AResource<T>* resource = (AResource<T>*)GetResource();
		resource->UnloadResource();
	}
}