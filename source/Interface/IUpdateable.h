#pragma once

#include <CCCommon.h>

namespace CitrusCore
{
	class CITRUS_CORE_API IUpdateable {
	public:
        virtual ~IUpdateable() = default;
		virtual void Update() = 0;
	};
}