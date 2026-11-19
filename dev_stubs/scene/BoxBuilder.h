#pragma once
#include <glm/glm.hpp>
#include <vector>

#include "scene/Mesh.h"

// STUB (real: Kuanysh). Appends one axis-aligned box (24 vertices, outward normals, CCW) to a vertex list.
namespace BoxBuilder {
void addBox(std::vector<Vertex>& v, std::vector<unsigned>& idx, const glm::vec3& center, const glm::vec3& size);
}
