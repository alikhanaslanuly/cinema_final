#include "Hall.h"
#include "BoxBuilder.h"
#include "SceneLayout.h"
#include "../core/Shader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <utility>

using glm::vec3;

namespace {
// Одна грань a,b,c,d (CCW с видимой стороны), нормаль считается из вершин.
// Видна только с лицевой стороны: с обратной её отсекает back-face culling.
void addQuad(MeshData& m, const vec3& a, const vec3& b, const vec3& c, const vec3& d) {
    const vec3 n = glm::normalize(glm::cross(b - a, c - b));
    const unsigned int base = static_cast<unsigned int>(m.vertices.size() / 8);
    const vec3 v[4] = {a, b, c, d};
    const float uv[4][2] = {{0, 0}, {1, 0}, {1, 1}, {0, 1}};
    for (int i = 0; i < 4; ++i) {
        m.vertices.insert(m.vertices.end(), {v[i].x, v[i].y, v[i].z, n.x, n.y, n.z, uv[i][0], uv[i][1]});
    }
    m.indices.insert(m.indices.end(), {base, base + 1, base + 2, base, base + 2, base + 3});
}
}

void Hall::init() {
    using namespace layout;
    const float W = kHallHalfW, H = kHallHeight;
    const float zF = kHallZFront, zB = kHallZBack;

    m_parts.clear();
    auto add = [this](const MeshData& d, const vec3& color) {
        Part p;
        p.mesh = Mesh(d);
        p.color = color;
        m_parts.push_back(std::move(p));
    };

    // пол и уступ: замкнутые боксы (сверху видны всегда, в том числе из orbit-обзора)
    MeshData floor;
    BoxBuilder::addBox(floor, vec3(-W, -0.2f, zF), vec3(W, 0.0f, zB));
    add(floor, vec3(0.38f, 0.10f, 0.14f));

    MeshData riser;
    BoxBuilder::addBox(riser, vec3(-W, 0.0f, kStepZ), vec3(W, kRowY[1], zB));
    add(riser, vec3(0.50f, 0.14f, 0.18f));

    // стены и потолок: односторонние плоскости, лицом внутрь зала.
    // Снаружи (orbit-камера) они невидимы -> виден интерьер.
    MeshData walls;
    addQuad(walls, vec3(-W, 0, zB), vec3(-W, 0, zF), vec3(-W, H, zF), vec3(-W, H, zB));  // левая  (нормаль +X)
    addQuad(walls, vec3( W, 0, zF), vec3( W, 0, zB), vec3( W, H, zB), vec3( W, H, zF));  // правая (нормаль -X)
    addQuad(walls, vec3(-W, 0, zF), vec3( W, 0, zF), vec3( W, H, zF), vec3(-W, H, zF));  // передняя (нормаль +Z)
    addQuad(walls, vec3( W, 0, zB), vec3(-W, 0, zB), vec3(-W, H, zB), vec3( W, H, zB));  // задняя (нормаль -Z)
    add(walls, vec3(0.35f, 0.35f, 0.40f));

    MeshData ceiling;
    addQuad(ceiling, vec3(-W, H, zF), vec3(W, H, zF), vec3(W, H, zB), vec3(-W, H, zB));  // потолок (нормаль -Y)
    add(ceiling, vec3(0.25f, 0.25f, 0.28f));
}

void Hall::draw(Shader& s) const {
    s.setMat4("uModel", glm::mat4(1.0f));
    s.setInt("uUseTexture", 0);
    s.setVec3("uEmissive", vec3(0.0f));
    for (const Part& p : m_parts) {
        s.setVec3("uColor", p.color);
        p.mesh.draw();
    }
}

float Hall::floorHeight(float /*x*/, float z) const {
    return (z >= layout::kStepZ) ? layout::kRowY[1] : 0.0f;
}

void Hall::bounds(glm::vec3& mn, glm::vec3& mx) const {
    using namespace layout;
    mn = vec3(-kHallHalfW, 0.0f, kHallZFront);
    mx = vec3(kHallHalfW, kHallHeight, kHallZBack);
}

void Hall::spawnPose(glm::vec3& eye, glm::vec3& lookAt) const {
    const float z = layout::kHallZBack - 1.0f;
    eye = vec3(0.0f, floorHeight(0.0f, z) + 1.7f, z);
    lookAt = vec3(0.0f, 2.0f, layout::kScreenZ);
}