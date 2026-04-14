// #include <iostream>
#include "camera.hpp"
#include "film.hpp"
#include "glm/common.hpp"
#include "glm/ext/vector_float3.hpp"
#include "sphere.hpp"
#include "thread_pool.hpp"
#include <glm/glm.hpp>


int main()
{
    ThreadPool thread_pool{};

    Film   film(1920, 1080);
    Camera camera{film, {0, 0, 1}, {0, 0, 0}, 90};
    Sphere sphere{{0, 0, 0}, 0.5f};

    glm::vec3 light_pos{3, 3, 2};

    thread_pool.parallelFor(film.getWidth(), film.getHeight(), [&](size_t x, size_t y) {
        auto ray    = camera.generateRay({x, y});
        auto result = sphere.intersect(ray);
        if (result.has_value()) {
            auto  hit_point = ray.hit(result.value());
            auto  normal    = glm::normalize(hit_point - sphere.centre);
            auto  L         = glm::normalize(light_pos - hit_point);
            float cosine    = glm::max(0.f, glm::dot(normal, L));

            film.setPixel(x, y, {cosine, cosine, cosine});
        }
    });
    thread_pool.wait();

    // auto now = std::chrono::high_resolution_clock::now();
    film.save("test.ppm");
    // auto time = std::chrono::high_resolution_clock::now() - now;
    // auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(time);
    // std::cout << "Time: " << ms.count() << std::endl;
    // 现版本要10196 ms，使用二进制只用200 ms
}