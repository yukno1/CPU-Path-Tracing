#pragma once

/*!
 * \file film.hpp
 * \brief 概述
 *
 *胶片类存储每个像素的颜色，并把图像存储为一张图片
 *
 */
///

#include <filesystem>
#include <vector>
#include "glm/glm.hpp"

class Film
{
public:
    Film(size_t width, size_t height);

    void save(const std::filesystem::path &filename);

    size_t getWidth() const { return width; }
    size_t getHeight() const { return height; }
    glm::vec3 getPixel(size_t x, size_t y) { return pixels[y * width + x]; }
    void setPixel(size_t x, size_t y, const glm::vec3 &color) { pixels[y * width + x] = color; }

private:
    size_t width, height;
    std::vector<glm::vec3> pixels;
};