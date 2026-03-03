#include <Util/Property.h>
#include <Base/Object.h>
#include <memory>

namespace CitrusCore
{
    PropertyBase* Object::GetProperty(std::string key) const
    {
        auto prop = m_properties.find(key);
       
        if (prop != m_properties.end())
        {
            return prop->second.get();
        }

        return nullptr;
    }
}