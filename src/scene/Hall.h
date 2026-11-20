#pragma once
// scene/Hall.h (Куаныш) — пол с уступом, стены, потолок. Y вверх, экран на -Z.
#include <vector>
#include <glm/glm.hpp>
#include "Mesh.h"

class Shader;

class Hall {
public:
    void init();                                          // после создания GL-контекста
    void draw(Shader& s) const;                           // сам выставляет uModel/uColor/uEmissive/uUseTexture
    float floorHeight(float x, float z) const;            // высота пола (уступ) в точке (x,z)
    void bounds(glm::vec3& mn, glm::vec3& mx) const;      // внутренний AABB зала
    void spawnPose(glm::vec3& eye, glm::vec3& lookAt) const;  // где игрок входит в зал

private:
    struct Part { Mesh mesh; glm::vec3 color; };
    std::vector<Part> m_parts;
};