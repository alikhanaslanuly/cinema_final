#include "SeatGrid.h"
#include "BoxBuilder.h"
#include "SceneLayout.h"
#include "../core/Shader.h"
#include <glm/gtc/matrix_transform.hpp>

namespace {
// Кресло в локальных координатах: начало = центр кресла на уровне пола.
MeshData buildSeatMesh() {
    MeshData m;
    using glm::vec3;
    BoxBuilder::addBox(m, vec3(-0.10f, 0.00f, -0.10f), vec3(0.10f, 0.35f, 0.10f));   // ножка
    BoxBuilder::addBox(m, vec3(-0.30f, 0.35f, -0.30f), vec3(0.30f, 0.45f, 0.30f));   // сиденье
    BoxBuilder::addBox(m, vec3(-0.30f, 0.45f,  0.22f), vec3(0.30f, 1.10f, 0.32f));   // спинка (сзади, +Z)
    BoxBuilder::addBox(m, vec3(-0.38f, 0.45f, -0.28f), vec3(-0.30f, 0.65f, 0.28f));  // подлокотник Л
    BoxBuilder::addBox(m, vec3( 0.30f, 0.45f, -0.28f), vec3( 0.38f, 0.65f, 0.28f));  // подлокотник П
    return m;
}
const glm::vec3 kFree(0.15f, 0.70f, 0.25f);
const glm::vec3 kTaken(0.80f, 0.15f, 0.15f);
const glm::vec3 kSelected(1.00f, 0.85f, 0.10f);
}

void SeatGrid::init() {
    using namespace layout;
    m_seats.clear();
    int id = 0;
    for (int r = 0; r < kRows; ++r) {
        for (int c = 0; c < kCols; ++c) {
            Seat s;
            s.id = id++;
            s.row = r;
            s.col = c;
            float cx = (c - (kCols - 1) / 2.0f) * kColStep;
            float cz = kRowZ[r], by = kRowY[r];
            s.aabbMin = glm::vec3(cx - 0.40f, by, cz - 0.35f);
            s.aabbMax = glm::vec3(cx + 0.40f, by + 1.10f, cz + 0.35f);
            s.eyePos = glm::vec3(cx, by + 1.20f, cz + 0.05f);   // сидящий зритель
            s.taken = false;
            m_seats.push_back(s);
        }
    }
    m_mesh = Mesh(buildSeatMesh());
    m_selected = -1;
}

int SeatGrid::pick(const Ray& ray) const {
    int best = -1;
    float bestT = 1e30f;
    for (const Seat& s : m_seats) {
        float t;
        if (rayAABB(ray, s.aabbMin, s.aabbMax, t) && t < bestT) {
            bestT = t;
            best = s.id;
        }
    }
    return best;
}

void SeatGrid::setTaken(int id, bool taken) {
    if (id >= 0 && id < static_cast<int>(m_seats.size())) m_seats[id].taken = taken;
}

void SeatGrid::setSelected(int id) {
    m_selected = (id >= 0 && id < static_cast<int>(m_seats.size())) ? id : -1;
}

void SeatGrid::draw(Shader& shader) const {
    shader.setInt("uUseTexture", 0);
    for (const Seat& s : m_seats) {
        glm::vec3 origin((s.aabbMin.x + s.aabbMax.x) * 0.5f, s.aabbMin.y, (s.aabbMin.z + s.aabbMax.z) * 0.5f);
        glm::vec3 color = s.taken ? kTaken : kFree;
        glm::vec3 emissive(0.0f);
        if (s.id == m_selected) {
            color = kSelected;
            emissive = kSelected * 0.35f;   // выбранное место слегка светится
        }
        shader.setMat4("uModel", glm::translate(glm::mat4(1.0f), origin));
        shader.setVec3("uColor", color);
        shader.setVec3("uEmissive", emissive);
        m_mesh.draw();
    }
}