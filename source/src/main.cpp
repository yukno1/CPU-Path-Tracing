// #include <iostream>
#include "camera.hpp"
#include "film.hpp"
#include "glm/common.hpp"
#include "glm/ext/vector_float3.hpp"
#include "model.hpp"
#include "plane.hpp"
#include "scene.hpp"
#include "sphere.hpp"
#include "thread_pool.hpp"
#include <glm/glm.hpp>
#include <iostream>
#include <ostream>





int main()
{
    ThreadPool thread_pool{};

    Film             film(1920, 1080);
    Camera           camera{film, {-1.6, 0, 0}, {0, 0, 0}, 90};
    std::atomic<int> count = 0;

    Sphere sphere{{0, 0, 0}, 0.5f};

    Plane plane{{0, 0, 0}, {0, 1, 0}};

    Model model("models\\simple_dragon.obj");

    Shape& shape = plane;

    Scene scene{};
    scene.addShape(&model, {0, 0, 0}, {1, 3, 2});
    scene.addShape(&sphere, {0, 0, 1.5}, {0.3, 0.3, 0.3});
    scene.addShape(&plane, {0, -0.5, 0});


    glm::vec3 light_pos{-1, 2, 1};

    thread_pool.parallelFor(film.getWidth(), film.getHeight(), [&](size_t x, size_t y) {
        auto ray      = camera.generateRay({x, y});
        auto hit_info = scene.intersect(ray);
        if (hit_info.has_value()) {
            auto  L      = glm::normalize(light_pos - hit_info->hit_point);
            float cosine = glm::max(0.f, glm::dot(hit_info->normal, L));

            film.setPixel(x, y, {cosine, cosine, cosine});
        }
        int n = ++count;
        if (n % film.getWidth() == 0) {
            std::cout << static_cast<float>(n) / (film.getWidth() * film.getHeight()) << std::endl;
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