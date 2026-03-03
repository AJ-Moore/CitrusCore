#pragma once

#include <Base/Object.h>
#include <Interface/IInitialisable.h>
#include <Interface/IManagedObject.h>
#include <Interface/IRenderable.h>
#include <Interface/IUpdateable.h>
#include <CCCommon.h>

namespace CitrusCore
{
	class CITRUS_CORE_API AService : public CitrusCore::Object, 
                                    protected CitrusCore::IInitialisable,
                                    protected CitrusCore::IUpdateable,
                                    protected CitrusCore::IRenderable
	{
		friend class Services;
	public:
        AService() = default;
        virtual ~AService() = default; 

		bool HasbeenInitialised() const { return m_bDoneInit; }
	protected:
		virtual bool Init() override = 0;
		virtual void Unload() override = 0;
		virtual void Update() override = 0;
		virtual void Render() override = 0;
		virtual void BeginRender() override{};
		virtual void RenderUI(){};

		bool m_bDoneInit = false;
	};
}