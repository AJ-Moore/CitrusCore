#pragma once

#include "Base/Object.h"
#include "Interface/IInitialisable.h"
#include "Interface/IRenderable.h"
#include "Interface/IUpdateable.h"
#include <CCCommon.h>
#include <Services/AService.h>
#include <memory>
#include <queue>
#include <unordered_map>

namespace CitrusCore
{
	class CITRUS_CORE_API Services : public CitrusCore::Object, 
										public CitrusCore::IInitialisable,
										public CitrusCore::IUpdateable,
										public CitrusCore::IRenderable
	{
	public:
		Services() = default;
		~Services() = default;
	
		// Delete the methods we don't want to allow
		Services(Services const&) = delete; // Copy constructor
		void operator=(Services const&) = delete; // Copy assignment operator

		void AddService(std::shared_ptr<AService> service);
	protected:
		virtual bool Init(); 
		virtual void Unload();
		virtual void Update();
		virtual void Render();

		std::queue<std::shared_ptr<AService>> m_awaitingInitialisation;
		std::unordered_map<UID, std::shared_ptr<AService>> m_services;
	};
}