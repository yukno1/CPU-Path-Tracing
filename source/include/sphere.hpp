#pragma once

#include "ray.hpp"
#include <optional>

struct Sphere
{
    glm::vec3 centre;
    float radius;

    /**
     * @return 有交点返回到球心的距离，没有返回Null
     */
    std::optional<float> intersect(const Ray &ray);
};
