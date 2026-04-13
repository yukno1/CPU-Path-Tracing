#include "film.hpp"
#include <fstream>

Film::Film(size_t width, size_t height)
    : width(width), height(height)
{
    pixels.resize(width * height);
}

void Film::save(const std::filesystem::path &filename)
{
    // PPM
    // P3 指定图片由3通道RGB组成
    // W H
    // 每个分量的最大值
    // color
    std::ofstream file(filename);
    file << "P3\n"
         << width << ' ' << height << '\n'
         << "255\n";

    for (size_t y = 0; y < height; y++)
    {
        for (size_t x = 0; x < width; x++)
        {
            const glm::vec3 &color = getPixels(x, y);
            glm::ivec3 color_i = glm::clamp(color * 255.f, 0.f, 255.f);
            file << color_i.x << ' ' << color_i.y << ' ' << color_i.z << '\n';
        }
    }
    // P3 使用ascii码
}
