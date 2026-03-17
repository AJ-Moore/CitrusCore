#pragma once 
#include <Base/PropertyDefinitionBase.h>
#include <Base/IPropertyRenderer.h>
#include <CCCommon.h>
#include <Interface/ISerialisable.h>

namespace CitrusCore {
    class CITRUS_CORE_API PropertyBase : ISerialisable
    {
    public:
        ~PropertyBase() = default;
        virtual void Serialise(StreamWriter& byteStream) override {}
        virtual void Deserialise(StreamReader& byteStream) override {}
        bool IsReadOnly() const { return m_bReadOnly; }

        std::string GetPropertyName() const { return m_propertyName; }
        virtual void RenderUI(IPropertyRenderer& renderer) = 0;

        virtual PropertyDefinitionBase* GetPropertyBase() = 0;
    protected:
        std::string m_propertyName;
        std::string m_typeName;
        bool m_bReadOnly = false;  
    };
}