#pragma once
#include <glm/glm.hpp>
#include <vector>

#include "core/Shader.h"
#include "picking/Ray.h"
#include "scene/Mesh.h"

// STUB (real: Kuanysh). Contract from the plan + init().
struct Seat {
    int id;                    // 0..9
    int row, col;              // 0..1, 0..4
    glm::vec3 aabbMin, aabbMax;
    glm::vec3 eyePos;          // spectator's eyes
    bool taken;
};

class SeatGrid {
public:
    void init();  // after the GL context exists
    const std::vector<Seat>& seats() const { return m_seats; }
    int pick(const Ray& ray) const;  // seat id or -1
    void setTaken(int id, bool taken);
    void setSelected(int id);  // -1 = none
    void draw(Shader& shader) const;

private:
    std::vector<Seat> m_seats;
    std::vector<Mesh> m_meshes;
    int m_selected = -1;
};
