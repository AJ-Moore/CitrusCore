#pragma once

#include <CCCommon.h>

namespace CitrusCore
{
	class CITRUS_CORE_API IInitialisable {
	public:
		virtual ~IInitialisable() = default;
		virtual bool Init() = 0;
		virtual void Unload() = 0;
	};
}