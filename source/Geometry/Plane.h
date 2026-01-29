#pragma once
#include <CCCommon.h>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <Geometry/Ray.h>

namespace CitrusCore 
{
    class CITRUS_CORE_API Plane
    {
    public: 
        Plane() = delete; 
        Plane(glm::vec3 position, glm::vec3 normal);
        glm::vec3 GetNormal() const { return m_normal; }
        glm::vec3 GetPosition() const { return m_position; }

        bool RayCast(Ray ray);
        float RayCast(Ray ray, float distance){}
    private:
        glm::vec3 m_position;
        glm::vec3 m_normal;
    };
}