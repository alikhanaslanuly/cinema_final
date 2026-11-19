#include "scene/Mesh.h"

#include <cstddef>

Mesh::~Mesh() { release(); }
Mesh::Mesh(Mesh&& o) noexcept : m_vao(o.m_vao), m_vbo(o.m_vbo), m_ebo(o.m_ebo), m_count(o.m_count) {
    o.m_vao = o.m_vbo = o.m_ebo = 0;
    o.m_count = 0;
}
Mesh& Mesh::operator=(Mesh&& o) noexcept {
    if (this != &o) {
        release();
        m_vao = o.m_vao; m_vbo = o.m_vbo; m_ebo = o.m_ebo; m_count = o.m_count;
        o.m_vao = o.m_vbo = o.m_ebo = 0;
        o.m_count = 0;
    }
    return *this;
}
void Mesh::release() {
    if (m_ebo) glDeleteBuffers(1, &m_ebo);
    if (m_vbo) glDeleteBuffers(1, &m_vbo);
    if (m_vao) glDeleteVertexArrays(1, &m_vao);
    m_vao = m_vbo = m_ebo = 0;
}
void Mesh::upload(const std::vector<Vertex>& v, const std::vector<unsigned>& idx) {
    release();
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(v.size() * sizeof(Vertex)), v.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(idx.size() * sizeof(unsigned)), idx.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, pos)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, normal)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, uv)));
    glBindVertexArray(0);
    m_count = static_cast<GLsizei>(idx.size());
}
void Mesh::draw() const {
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, m_count, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}
