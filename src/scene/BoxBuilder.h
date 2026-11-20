#pragma once
// scene/BoxBuilder.h (Куаныш) — процедурные боксы с нормалями и UV
#include <glm/glm.hpp>
#include "Mesh.h"

namespace BoxBuilder {
// Добавляет бокс [bmin, bmax] в MeshData (24 вершины, 36 индексов, CCW снаружи).
// uvScale: сколько повторов текстуры на 1 метр (для пола/стен).
void addBox(MeshData& out, const glm::vec3& bmin, const glm::vec3& bmax, float uvScale = 1.0f);

// Квад, лежащий в плоскости z = const, смотрит в +Z (для экрана).
void addQuadZ(MeshData& out, float x0, float x1, float y0, float y1, float z);
}