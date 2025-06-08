#pragma once

//#include "Resources/ResourceHandle.h"
#include <CCCommon.h>

namespace CitrusCore
{
	class CITRUS_CORE_API AResourceBase{
	public:
		virtual bool LoadResource(std::string path) = 0;
		virtual void UnloadResource() = 0;
		virtual void SetResourceLocation(const std::string& path) = 0;
	};

	template <typename T>
	class ResourceHandle;

	template <class T>
	class CITRUS_CORE_API AResource : public AResourceBase
	{
		friend class ResourceHandle<T>;
	public:
		virtual ~AResource() { UnloadResource(); }
		const std::string& GetResourceLocation() { return m_resourceLocation; }
	protected:
		virtual bool LoadResource(std::string path) override = 0;
		virtual void UnloadResource() override = 0;
		virtual void SetResourceLocation(const std::string& path) override final { m_resourceLocation = path; }
	private:
		std::string m_resourceLocation;
	};
}