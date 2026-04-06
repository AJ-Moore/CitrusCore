#pragma once
#include <Base/IPropertyRenderer.h>
#include <CCCommon.h>
#include <Events/Event.h>

namespace CitrusCore
{
    class PropertyBase;
    class CITRUS_CORE_API IPropReadOnly{};
    class CITRUS_CORE_API PropertyDefinitionBase{
    protected:
        bool m_readOnly = false;
    public:
        constexpr PropertyDefinitionBase(bool readOnly) : m_readOnly(readOnly) {}
        constexpr bool IsReadOnly() const { return m_readOnly; }
        virtual void RenderUI(PropertyBase* property, IPropertyRenderer&) {}
    };
}
