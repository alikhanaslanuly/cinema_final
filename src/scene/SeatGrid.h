#pragma once
// scene/SeatGrid.h (Куаныш) — контракт: CONTRACT.md
#include <vector>
#include <glm/glm.hpp>
#include "../picking/Ray.h"
#include "Mesh.h"

class Shader;

struct Seat {
    int id;                 // 0..9, seats()[id].id == id
    int row, col;           // 0..1, 0..4
    glm::vec3 aabbMin, aabbMax;
    glm::vec3 eyePos;       // позиция глаз зрителя
    bool taken;             // занято
};

class SeatGrid {
public:
    void init();                                       // после создания GL-контекста
    const std::vector<Seat>& seats() const { return m_seats; }
    int pick(const Ray& ray) const;                    // id ближайшего кресла под лучом или -1
    void setTaken(int id, bool taken);
    void setSelected(int id);                          // -1 = снять подсветку
    void draw(Shader& shader) const;

private:
    std::vector<Seat> m_seats;
    Mesh m_mesh;
    int m_selected = -1;
};