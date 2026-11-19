#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

// STUB (real: Kuanysh). Vertex layout contract: location 0 = pos, 1 = normal, 2 = uv.
struct Vertex {
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec2 uv;
};

class Mesh {
public:
    Mesh() = default;
    ~Mesh();
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;
    Mesh(Mesh&& o) noexcept;
    Mesh& operator=(Mesh&& o) noexcept;
    void upload(const std::vector<Vertex>& v, const std::vector<unsigned>& idx);
    void draw() const;

private:
    void release();
    GLuint m_vao = 0, m_vbo = 0, m_ebo = 0;
    GLsizei m_count = 0;
};
