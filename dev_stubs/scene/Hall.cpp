#include "scene/Hall.h"

#include "scene/BoxBuilder.h"

void Hall::init() {
    auto add = [&](glm::vec3 c, glm::vec3 size, glm::vec3 color) {
        std::vector<Vertex> v;
        std::vector<unsigned> i;
        BoxBuilder::addBox(v, i, c, size);
        Part p;
        p.mesh.upload(v, i);
        p.color = color;
        m_parts.push_back(std::move(p));
    };
    add({0, -0.25f, 0}, {14, 0.5f, 20}, {0.35f, 0.30f, 0.28f});        // main floor (top at y = 0)
    add({0, 0.3f, 6.5f}, {14, 0.6f, 7}, {0.42f, 0.36f, 0.32f});        // rear tier (top at y = 0.6, z 3..10)
    add({-7.25f, 3.5f, 0}, {0.5f, 7, 20.5f}, {0.45f, 0.20f, 0.22f});   // left wall
    add({7.25f, 3.5f, 0}, {0.5f, 7, 20.5f}, {0.45f, 0.20f, 0.22f});    // right wall
    add({0, 3.5f, 10.25f}, {15, 7, 0.5f}, {0.40f, 0.18f, 0.20f});      // back wall
    add({0, 3.5f, -10.25f}, {15, 7, 0.5f}, {0.20f, 0.20f, 0.24f});     // front wall
    add({0, 7.25f, 0}, {15, 0.5f, 20.5f}, {0.55f, 0.55f, 0.58f});      // ceiling
}

void Hall::draw(Shader& s) const {
    s.setMat4("uModel", glm::mat4(1.0f));
    s.setVec3("uEmissive", glm::vec3(0.0f));
    s.setInt("uUseTexture", 0);
    for (const auto& p : m_parts) {
        s.setVec3("uColor", p.color);
        p.mesh.draw();
    }
}

float Hall::floorHeight(float, float z) const { return z >= 3.0f ? 0.6f : 0.0f; }
void Hall::bounds(glm::vec3& mn, glm::vec3& mx) const {
    mn = glm::vec3(-7.0f, 0.0f, -10.0f);
    mx = glm::vec3(7.0f, 7.0f, 10.0f);
}
void Hall::spawnPose(glm::vec3& eye, glm::vec3& lookAt) const {
    eye = glm::vec3(0.0f, 2.3f, 9.0f);
    lookAt = glm::vec3(0.0f, 3.0f, -9.0f);
}
