#pragma once

#include "ray.hpp"
#include <limits>
#include <optional>

struct Shape
{
    /**
     * [t_min, t_max] 以外视为不相交
     * @return 有交点返回Hitinfo，没有返回Null
     */
    virtual std::optional<HitInfo> intersect(
        const Ray& ray, float t_min = 1e-5,
        float t_max = std::numeric_limits<float>::infinity()) const = 0;
};