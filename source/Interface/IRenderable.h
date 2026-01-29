#pragma once

#include <CCCommon.h>

namespace CitrusCore
{
	class CITRUS_CORE_API IRenderable {
	public:
        virtual ~IRenderable() = default;
		virtual void Render() = 0;
		virtual void BeginRender(){};
	};
}