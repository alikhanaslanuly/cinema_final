#include "scene/BoxBuilder.h"

#include <cmath>

namespace BoxBuilder {
void addBox(std::vector<Vertex>& v, std::vector<unsigned>& idx, const glm::vec3& c, const glm::vec3& size) {
    const glm::vec3 h = size * 0.5f;
    // normal n, tangent a, bitangent b with cross(a, b) == n  -> CCW seen from outside
    const glm::vec3 N[6] = {{1, 0, 0}, {-1, 0, 0}, {0, 1, 0}, {0, -1, 0}, {0, 0, 1}, {0, 0, -1}};
    const glm::vec3 A[6] = {{0, 1, 0}, {0, 0, 1}, {0, 0, 1}, {1, 0, 0}, {1, 0, 0}, {0, 1, 0}};
    const glm::vec3 B[6] = {{0, 0, 1}, {0, 1, 0}, {1, 0, 0}, {0, 0, 1}, {0, 1, 0}, {1, 0, 0}};
    for (int f = 0; f < 6; ++f) {
        const glm::vec3 n = N[f], a = A[f], b = B[f];
        float hn = std::fabs(n.x) * h.x + std::fabs(n.y) * h.y + std::fabs(n.z) * h.z;
        float ha = std::fabs(a.x) * h.x + std::fabs(a.y) * h.y + std::fabs(a.z) * h.z;
        float hb = std::fabs(b.x) * h.x + std::fabs(b.y) * h.y + std::fabs(b.z) * h.z;
        unsigned base = static_cast<unsigned>(v.size());
        const float sa[4] = {-1, 1, 1, -1}, sb[4] = {-1, -1, 1, 1};
        for (int i = 0; i < 4; ++i) {
            Vertex vt;
            vt.pos = c + n * hn + a * (sa[i] * ha) + b * (sb[i] * hb);
            vt.normal = n;
            vt.uv = glm::vec2((sa[i] * 0.5f + 0.5f) * ha, (sb[i] * 0.5f + 0.5f) * hb);
            v.push_back(vt);
        }
        const unsigned order[6] = {0, 1, 2, 0, 2, 3};
        for (unsigned o : order) idx.push_back(base + o);
    }
}
}  // namespace BoxBuilder
