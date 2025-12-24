#include <Geometry/Plane.h>
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>

namespace CitrusCore {
    Plane::Plane(glm::vec3 position, glm::vec3 normal)
    {
        m_normal = normal;
    }

    bool Plane::RayCast(Ray ray)
    {
        if (glm::dot((ray.GetOrigin() - m_position), m_normal) > 0)
        {
            return glm::dot(ray.GetDirection(), m_normal) < 0;
        }

        return false;
    }
}