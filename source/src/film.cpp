#include "film.hpp"
#include <fstream>

Film::Film(size_t width, size_t height)
    : width(width), height(height)
{
    pixels.resize(width * height);
}

/**
 * 存储为ppm格式：
 * PPM
 * P3 指定图片由3通道RGB组成 （P3 使用ascii码，P6 使用二进制）
 * W H
 * 每个分量的最大值
 * color
 */
void Film::save(const std::filesystem::path &filename)
{
    std::ofstream file(filename, std::ios::binary);
    file << "P6\n"
         << width << ' ' << height << '\n'
         << "255\n";

    for (size_t y = 0; y < height; y++)
    {
        for (size_t x = 0; x < width; x++)
        {
            const glm::vec3 &color = getPixel(x, y);
            glm::u8vec3 color_u8 = glm::clamp(color * 255.f, 0.f, 255.f);
            file << color_u8.x << color_u8.y << color_u8.z;
        }
    }
}
