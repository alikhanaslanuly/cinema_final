#include "scene/SeatGrid.h"

#include <algorithm>
#include <cmath>
#include <limits>

#include "scene/BoxBuilder.h"

namespace {
bool rayAabb(const Ray& r, const glm::vec3& mn, const glm::vec3& mx, float& tHit) {
    float t0 = 0.0f, t1 = std::numeric_limits<float>::max();
    for (int a = 0; a < 3; ++a) {
        float o = a == 0 ? r.origin.x : (a == 1 ? r.origin.y : r.origin.z);
        float d = a == 0 ? r.dir.x : (a == 1 ? r.dir.y : r.dir.z);
        float lo = a == 0 ? mn.x : (a == 1 ? mn.y : mn.z);
        float hi = a == 0 ? mx.x : (a == 1 ? mx.y : mx.z);
        if (std::fabs(d) < 1e-8f) {
            if (o < lo || o > hi) return false;
        } else {
            float ta = (lo - o) / d, tb = (hi - o) / d;
            if (ta > tb) std::swap(ta, tb);
            t0 = std::max(t0, ta);
            t1 = std::min(t1, tb);
            if (t0 > t1) return false;
        }
    }
    tHit = t0;
    return true;
}
}  // namespace

void SeatGrid::init() {
    for (int row = 0; row < 2; ++row) {
        for (int col = 0; col < 5; ++col) {
            Seat s;
            s.id = row * 5 + col;
            s.row = row;
            s.col = col;
            float x = (col - 2) * 1.6f, z = row == 0 ? 0.5f : 5.0f, y = row == 0 ? 0.0f : 0.6f;
            s.aabbMin = glm::vec3(x - 0.38f, y, z - 0.32f);
            s.aabbMax = glm::vec3(x + 0.38f, y + 1.25f, z + 0.38f);
            s.eyePos = glm::vec3(x, y + 1.15f, z + 0.1f);
            s.taken = (s.id == 1 || s.id == 6 || s.id == 8);  // demo: some seats are already sold
            m_seats.push_back(s);

            std::vector<Vertex> v;
            std::vector<unsigned> i;
            BoxBuilder::addBox(v, i, {x, y + 0.20f, z}, {0.5f, 0.4f, 0.5f});          // base
            BoxBuilder::addBox(v, i, {x, y + 0.46f, z}, {0.6f, 0.12f, 0.6f});         // seat
            BoxBuilder::addBox(v, i, {x, y + 0.85f, z + 0.30f}, {0.6f, 0.8f, 0.12f}); // back (faces the screen at -Z)
            BoxBuilder::addBox(v, i, {x - 0.34f, y + 0.63f, z}, {0.08f, 0.26f, 0.55f});  // armrests
            BoxBuilder::addBox(v, i, {x + 0.34f, y + 0.63f, z}, {0.08f, 0.26f, 0.55f});
            Mesh m;
            m.upload(v, i);
            m_meshes.push_back(std::move(m));
        }
    }
}

int SeatGrid::pick(const Ray& ray) const {
    int best = -1;
    float bestT = std::numeric_limits<float>::max();
    for (const auto& s : m_seats) {
        float t;
        if (rayAabb(ray, s.aabbMin, s.aabbMax, t) && t < bestT) {
            bestT = t;
            best = s.id;
        }
    }
    return best;
}

void SeatGrid::setTaken(int id, bool taken) {
    if (id >= 0 && id < static_cast<int>(m_seats.size())) m_seats[id].taken = taken;
}
void SeatGrid::setSelected(int id) { m_selected = id; }

void SeatGrid::draw(Shader& s) const {
    s.setMat4("uModel", glm::mat4(1.0f));
    s.setInt("uUseTexture", 0);
    for (size_t i = 0; i < m_seats.size(); ++i) {
        glm::vec3 col(0.2f, 0.75f, 0.3f), em(0.0f);
        if (m_seats[i].taken) col = glm::vec3(0.8f, 0.15f, 0.15f);
        if (static_cast<int>(i) == m_selected) { col = glm::vec3(1.0f, 0.85f, 0.2f); em = glm::vec3(0.35f, 0.28f, 0.0f); }
        s.setVec3("uColor", col);
        s.setVec3("uEmissive", em);
        m_meshes[i].draw();
    }
}
