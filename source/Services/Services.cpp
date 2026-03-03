#include <Services/AService.h>
#include <Services/Services.h>
#include <memory>

namespace CitrusCore{

    bool Services::Init()
    {
        while (!m_awaitingInitialisation.empty())
        {
            std::shared_ptr<AService> service = m_awaitingInitialisation.front();
            m_awaitingInitialisation.pop();
            if (service->Init())
            {
                service->m_bDoneInit = true;
            }
        }

        return true;
    }

    void Services::AddService(std::shared_ptr<AService> service)
    {
        m_services.emplace(service->GetUID(), service);
        m_awaitingInitialisation.push(service);
    }

    void Services::Unload()
    {
        for (auto& service : m_services)
        {
            service.second->Unload();
        }
    }

    void Services::Update()
    {
        for (auto& service : m_services)
        {
            service.second->Update();
        }
    }

    void Services::Render()
    {
        for (auto& service : m_services)
        {
            service.second->Render();
        }
    }

    void Services::BeginRender()
    {
        for (auto& service : m_services)
        {
            service.second->BeginRender();
        }
    }

    void Services::RenderUI() 
    {
        for (auto& service : m_services)
        {
            service.second->RenderUI();
        }
    }
}