#pragma once

#include "ray.hpp"
#include "shape.hpp"

struct Sphere : public Shape
{
    Sphere(const glm::vec3& centre, float radius)
        : centre(centre)
        , radius(radius)
    {}
    glm::vec3 centre;
    float     radius;

    std::optional<HitInfo> intersect(const Ray& ray, float t_min, float t_max) const override;
};
