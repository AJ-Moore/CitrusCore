#pragma once

#include <CCCommon.h>

namespace CitrusCore
{
	template <class T>
	class CITRUS_CORE_API ResourceHandle
	{
		//friend class ResourceGroup;
	public:
		ResourceHandle() = delete;
		ResourceHandle(std::string path) { m_path = path; }
		std::weak_ptr<T> GetResource() { return m_resource; }
	private: 
		std::string m_path;
		std::unique_ptr<T> m_resource = nullptr;
		bool m_bLoaded = false;
	};

	template <class T>
	using ResourcePtr = std::shared_ptr<ResourceHandle<T>>;
}