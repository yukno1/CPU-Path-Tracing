#pragma once

#include "glm/ext/vector_float3.hpp"
#include <glm/glm.hpp>

struct Ray
{
    glm::vec3 origin;
    glm::vec3 direction;

    /**
     * @param t 传播的距离
     */
    glm::vec3 hit(float t) const { return origin + t * direction; }
};
