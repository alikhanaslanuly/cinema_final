#pragma once
#include <glm/glm.hpp>
#include <vector>

#include "core/Shader.h"
#include "scene/Mesh.h"

// STUB (real: Kuanysh). Hall: floor with two tiers, walls, ceiling. Right-handed, Y up, screen at -Z.
class Hall {
public:
    void init();  // after the GL context exists
    void draw(Shader& s) const;  // sets uModel, uColor, uEmissive, uUseTexture itself
    float floorHeight(float x, float z) const;                       // walkable height at (x,z)
    void bounds(glm::vec3& mn, glm::vec3& mx) const;                 // inner AABB of the hall
    void spawnPose(glm::vec3& eye, glm::vec3& lookAt) const;         // where the player enters

private:
    struct Part { Mesh mesh; glm::vec3 color; };
    std::vector<Part> m_parts;
};
