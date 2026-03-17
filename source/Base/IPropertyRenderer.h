#pragma once

#include <CCCommon.h>
#include <cstdint>
#include <glm/fwd.hpp>

namespace CitrusCore {
    class Transform;
    class PropertyBase;
    struct CITRUS_CORE_API IPropertyRenderer{
        virtual void Draw(PropertyBase* property, int8_t& value) = 0;
        virtual void Draw(PropertyBase* property, uint8_t& value) = 0;
        virtual void Draw(PropertyBase* property, int16_t& value) = 0;
        virtual void Draw(PropertyBase* property, uint16_t& value) = 0;
        virtual void Draw(PropertyBase* property, int32_t& value) = 0;
        virtual void Draw(PropertyBase* property, uint32_t& value) = 0;
        virtual void Draw(PropertyBase* property, int64_t& value) = 0;
        virtual void Draw(PropertyBase* property, uint64_t& value) = 0;
        virtual void Draw(PropertyBase* property, float& value) = 0;
        virtual void Draw(PropertyBase* property, double& value) = 0;
        virtual void Draw(PropertyBase* property, bool& value) = 0;
        virtual void Draw(PropertyBase* property, glm::vec3& value) = 0;
        virtual void Draw(PropertyBase* property, glm::mat4& value) = 0;
        virtual void Draw(PropertyBase* property, Transform& value) = 0;
    };
}
