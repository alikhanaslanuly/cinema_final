#include "scene/Screen.h"

#include <cmath>

#include "scene/BoxBuilder.h"

void Screen::init() {
    std::vector<Vertex> v;
    std::vector<unsigned> i;
    BoxBuilder::addBox(v, i, center(), {10.0f, 4.5f, 0.1f});
    m_mesh.upload(v, i);
}

void Screen::draw(Shader& s, float t) const {
    glm::vec3 c(0.5f + 0.5f * std::sin(t * 0.7f), 0.5f + 0.5f * std::sin(t * 1.1f + 1.0f), 0.5f + 0.5f * std::sin(t * 1.7f + 2.0f));
    s.setMat4("uModel", glm::mat4(1.0f));
    s.setVec3("uColor", c * 0.3f);
    s.setVec3("uEmissive", c * 0.8f);
    s.setInt("uUseTexture", 0);
    m_mesh.draw();
}
