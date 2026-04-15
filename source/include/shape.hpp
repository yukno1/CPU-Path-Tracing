#pragma once

#include "ray.hpp"
#include <optional>

struct Shape
{
    /**
     * [t_min, t_max] 以外视为不相交
     * @return 有交点返回Hitinfo，没有返回Null
     */
    virtual std::optional<HitInfo> intersect(const Ray& ray, float t_min, float t_max) const = 0;
};