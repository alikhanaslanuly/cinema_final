#include "BoxBuilder.h"

namespace {
void pushFace(MeshData& m, const glm::vec3 v[4], const glm::vec3& n, float uw, float uh) {
    unsigned int base = static_cast<unsigned int>(m.vertices.size() / 8);
    const float uvs[4][2] = {{0, 0}, {uw, 0}, {uw, uh}, {0, uh}};
    for (int i = 0; i < 4; ++i) {
        m.vertices.insert(m.vertices.end(), {v[i].x, v[i].y, v[i].z, n.x, n.y, n.z, uvs[i][0], uvs[i][1]});
    }
    m.indices.insert(m.indices.end(), {base, base + 1, base + 2, base, base + 2, base + 3});
}
}

namespace BoxBuilder {

void addBox(MeshData& m, const glm::vec3& a, const glm::vec3& b, float s) {
    const float dx = (b.x - a.x) * s, dy = (b.y - a.y) * s, dz = (b.z - a.z) * s;
    // каждая грань: 4 вершины против часовой стрелки, если смотреть снаружи
    { glm::vec3 v[4] = {{a.x,a.y,b.z},{b.x,a.y,b.z},{b.x,b.y,b.z},{a.x,b.y,b.z}}; pushFace(m, v, { 0, 0, 1}, dx, dy); } // +Z
    { glm::vec3 v[4] = {{b.x,a.y,a.z},{a.x,a.y,a.z},{a.x,b.y,a.z},{b.x,b.y,a.z}}; pushFace(m, v, { 0, 0,-1}, dx, dy); } // -Z
    { glm::vec3 v[4] = {{b.x,a.y,b.z},{b.x,a.y,a.z},{b.x,b.y,a.z},{b.x,b.y,b.z}}; pushFace(m, v, { 1, 0, 0}, dz, dy); } // +X
    { glm::vec3 v[4] = {{a.x,a.y,a.z},{a.x,a.y,b.z},{a.x,b.y,b.z},{a.x,b.y,a.z}}; pushFace(m, v, {-1, 0, 0}, dz, dy); } // -X
    { glm::vec3 v[4] = {{a.x,b.y,b.z},{b.x,b.y,b.z},{b.x,b.y,a.z},{a.x,b.y,a.z}}; pushFace(m, v, { 0, 1, 0}, dx, dz); } // +Y
    { glm::vec3 v[4] = {{a.x,a.y,a.z},{b.x,a.y,a.z},{b.x,a.y,b.z},{a.x,a.y,b.z}}; pushFace(m, v, { 0,-1, 0}, dx, dz); } // -Y
}

void addQuadZ(MeshData& m, float x0, float x1, float y0, float y1, float z) {
    glm::vec3 v[4] = {{x0,y0,z},{x1,y0,z},{x1,y1,z},{x0,y1,z}};
    pushFace(m, v, {0, 0, 1}, 1.0f, 1.0f);
}

} // namespace BoxBuilder