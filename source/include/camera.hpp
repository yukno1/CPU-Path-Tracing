#pragma once

#include "film.hpp"
#include "ray.hpp"

class Camera
{
public:
    /**
     * @param viewpoint 相机看向的点
     * @param fovy Y方向上的视野
     */
    Camera(Film &film, const glm::vec3 &pos, const glm::vec3 &viewpoint, float fovy);

    /**
     * @param pixel_coord 屏幕空间中的像素坐标
     * @param offset 像素内的偏移
     *
     * @return 返回世界空间中的一条光线
     */
    Ray generateRay(const glm::ivec2 &pixel_coord, const glm::vec2 &offset = {0.5, 0.5});

    Film &getFilm() { return film; }
    const Film &getFilm() const { return film; }

private:
    Film &film;
    glm::vec3 pos;

    // 从裁剪空间到相机空间
    glm::mat4 camera_from_clip;
    // 从相机空间到世界空间
    glm::mat4 world_from_camera;
};