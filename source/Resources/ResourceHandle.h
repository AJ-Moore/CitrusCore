#pragma once

#include <Resources/AResource.h>
#include <CCCommon.h>
#include <memory>
#include <string>

namespace CitrusCore
{
	template<class T> 
	class ResourceGroup;

	class CITRUS_CORE_API ResourceHandleBase{
	public:
		virtual const std::string& GetPath () const = 0;
	};

	template <class T>
	class CITRUS_CORE_API ResourceHandle : ResourceHandleBase
	{
		static_assert(std::is_base_of<AResource<T>, T>::value, 
			"T must derive from AResource");
		friend class ResourceGroup<T>;
	public:
		ResourceHandle() = delete;
		ResourceHandle(std::string path) { m_path = path; }
		T* GetResource() { return m_resource.get(); }

		virtual const std::string& GetPath () const override { return m_path; }
	protected:
		template<class P>
		void Load(); 
		virtual void Unload();
	private: 
		std::string m_path;
		std::shared_ptr<T> m_resource = nullptr;
		bool m_bLoaded = false;
	};

	template <class T>
	using ResourcePtr = std::shared_ptr<ResourceHandle<T>>;

	template<class T>
	template<class P>
	void ResourceHandle<T>::Load() 
	{
		m_resource = std::static_pointer_cast<T>(std::make_shared<P>());
		AResource<T>* resource = static_cast<AResource<T>*>(GetResource());
		m_bLoaded = resource->LoadResource(m_path);
	}

	template<class T>
	void ResourceHandle<T>::Unload() 
	{
		AResource<T>* resource = (AResource<T>*)GetResource();
		resource->UnloadResource();
	}
}