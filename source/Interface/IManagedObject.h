#pragma once

#include <CCCommon.h>

namespace CitrusCore
{
	class CITRUS_CORE_API IManagedObject {
	public:
		virtual bool Init() = 0;
		virtual void Unload() = 0;
		virtual void Update() = 0;
		virtual void Render() = 0;
	};
}