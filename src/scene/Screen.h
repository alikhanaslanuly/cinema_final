#pragma once
// scene/Screen.h (Куаныш) — экран на передней стене, слегка светится
#include <glm/glm.hpp>
#include "Mesh.h"

class Shader;

class Screen {
public:
    void init();
    void draw(Shader& s, float time) const;   // time = секунды анимации (замораживается клавишей P)
    glm::vec3 center() const;                 // куда смотреть из кресла

private:
    Mesh m_mesh;
};