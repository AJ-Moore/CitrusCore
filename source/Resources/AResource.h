#pragma once

#include <CCCommon.h>

namespace CitrusCore
{
	class CITRUS_CORE_API AResourceBase{};

	template <class T>
	class CITRUS_CORE_API AResource : public AResourceBase
	{
	public:
		virtual ~AResource() { }
		const std::string& GetResourceLocation() { return m_resourceLocation; }
	protected:
		virtual bool LoadResource(std::string path) = 0;
		void SetResourceLocation(const std::string& path) { m_resourceLocation = path; }
	private:
		std::string m_resourceLocation;
	};
}