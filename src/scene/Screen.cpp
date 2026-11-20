#include "Screen.h"
#include "BoxBuilder.h"
#include "SceneLayout.h"
#include "../core/Shader.h"
#include <cmath>

void Screen::init() {
    MeshData m;
    // тонкий бокс перед передней стеной (передняя грань на z = kScreenZ)
    BoxBuilder::addBox(m, glm::vec3(-layout::kScreenHalfW, layout::kScreenY0, layout::kScreenZ - 0.03f),
                          glm::vec3(layout::kScreenHalfW, layout::kScreenY1, layout::kScreenZ));
    m_mesh = Mesh(m);
}

void Screen::draw(Shader& s, float time) const {
    // «кадр»: плавно меняющийся цвет, светится через uEmissive
    glm::vec3 e(0.5f + 0.5f * std::cos(time * 0.6f),
                0.5f + 0.5f * std::cos(time * 0.6f + 2.0f),
                0.5f + 0.5f * std::cos(time * 0.6f + 4.0f));
    s.setMat4("uModel", glm::mat4(1.0f));
    s.setInt("uUseTexture", 0);
    s.setVec3("uColor", glm::vec3(0.05f, 0.05f, 0.06f));
    s.setVec3("uEmissive", e * 0.8f);
    m_mesh.draw();
}

glm::vec3 Screen::center() const {
    return glm::vec3(0.0f, (layout::kScreenY0 + layout::kScreenY1) * 0.5f, layout::kScreenZ);
}