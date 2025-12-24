#pragma once

#include <Interface/IInitialisable.h>
#include <Interface/IRenderable.h>
#include <Interface/IUpdateable.h>
#include <CCCommon.h>

namespace CitrusCore
{
	class CITRUS_CORE_API IManagedObject : 
						public IInitialisable,
						public IUpdateable, 
						public IRenderable
	{
	public:
		virtual bool Init() = 0;
		virtual void Unload() = 0;
		virtual void Update() = 0;
		virtual void Render() = 0;
	};
}