#include "scene.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/trigonometric.hpp"
#include "ray.hpp"
#include <optional>

void Scene::addShape(const Shape* shape, const glm::vec3& pos, const glm::vec3& scale,
                     const glm::vec3& rotate)
{
    glm::mat4 world_from_object = glm::translate(glm::mat4(1.f), pos) *
                                  glm::rotate(glm::mat4(1.f), glm::radians(rotate.z), {0, 0, 1}) *
                                  glm::rotate(glm::mat4(1.f), glm::radians(rotate.y), {0, 1, 0}) *
                                  glm::rotate(glm::mat4(1.f), glm::radians(rotate.x), {1, 0, 0}) *
                                  glm::scale(glm::mat4(1.f), scale);
    instances.push_back(ShapeInstance{shape, world_from_object, glm::inverse(world_from_object)});
}

std::optional<HitInfo> Scene::intersect(const Ray& ray, float t_min, float t_max) const
{
    std::optional<HitInfo> cloest_hit_info{};
    const ShapeInstance*   cloest_instance = nullptr;

    for (const auto& instance : instances) {
        auto ray_object = ray.objectFromWorld(instance.object_from_world);
        auto hit_info   = instance.shape->intersect(ray_object, t_min, t_max);
        if (hit_info.has_value()) {
            cloest_hit_info = hit_info;
            t_max           = hit_info->t;
            cloest_instance = &instance;
        }
    }

    if (cloest_instance) {
        cloest_hit_info->hit_point =
            cloest_instance->world_from_object * glm::vec4(cloest_hit_info->hit_point, 1.f);
        cloest_hit_info->normal = glm::transpose(cloest_instance->object_from_world) *
                                  glm::vec4(cloest_hit_info->normal, 0.f);
    }
    return cloest_hit_info;
}