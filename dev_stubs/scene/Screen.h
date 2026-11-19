#pragma once
#include <glm/glm.hpp>

#include "core/Shader.h"
#include "scene/Mesh.h"

// STUB (real: Kuanysh). Cinema screen on the front wall, slightly emissive.
class Screen {
public:
    void init();
    void draw(Shader& s, float time) const;  // time = animation seconds (frozen by P)
    glm::vec3 center() const { return glm::vec3(0.0f, 3.8f, -9.9f); }

private:
    Mesh m_mesh;
};
