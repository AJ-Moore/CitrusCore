#include <CCCommon.h>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>

namespace CitrusCore 
{
    struct CITRUS_CORE_API Ray
    {
    public:
        Ray() = delete; 
        Ray(glm::vec3 origin, glm::vec3 direction) : m_origin(origin), m_direction(direction){}

        glm::vec3 GetOrigin() { return m_origin; }
        glm::vec3 GetDirection() { return m_direction; }
    private:
        glm::vec3 m_origin; 
        glm::vec3 m_direction;
    };
}