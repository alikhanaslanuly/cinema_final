#pragma once
// picking/Ray.h  (Куаныш)
#include <glm/glm.hpp>
#include <algorithm>

struct Ray {
    glm::vec3 origin{0.0f};
    glm::vec3 dir{0.0f, 0.0f, -1.0f}; // нормализованное направление
};

// Ray-AABB (slab method). Возвращает true при пересечении, tHit = расстояние вдоль луча.
inline bool rayAABB(const Ray& ray, const glm::vec3& bmin, const glm::vec3& bmax, float& tHit) {
    float tMin = 0.0f;
    float tMax = 1e30f;
    for (int i = 0; i < 3; ++i) {
        if (glm::abs(ray.dir[i]) < 1e-8f) {
            // луч параллелен плоскостям слоя: должен лежать внутри слоя
            if (ray.origin[i] < bmin[i] || ray.origin[i] > bmax[i]) return false;
        } else {
            float inv = 1.0f / ray.dir[i];
            float t1 = (bmin[i] - ray.origin[i]) * inv;
            float t2 = (bmax[i] - ray.origin[i]) * inv;
            if (t1 > t2) std::swap(t1, t2);
            tMin = std::max(tMin, t1);
            tMax = std::min(tMax, t2);
            if (tMin > tMax) return false;
        }
    }
    tHit = tMin;
    return true;
}