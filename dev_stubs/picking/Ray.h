#pragma once
#include <glm/glm.hpp>

// STUB (real one: Kuanysh, src/picking). Only the fields origin/dir are part of the contract.
struct Ray {
    glm::vec3 origin{0.0f};
    glm::vec3 dir{0.0f, 0.0f, -1.0f};  // unit length
};
