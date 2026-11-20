#pragma once
// scene/Mesh.h (Куаныш)
// Вершина: pos(3) normal(3) uv(2) -> location 0,1,2
#include <vector>
#include <glad/glad.h>

struct MeshData {
    std::vector<float> vertices;      // по 8 float на вершину
    std::vector<unsigned int> indices;
};

class Mesh {
public:
    Mesh() = default;
    explicit Mesh(const MeshData& data);
    ~Mesh();
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;
    Mesh(Mesh&& o) noexcept;
    Mesh& operator=(Mesh&& o) noexcept;

    void draw() const;
private:
    void release();
    GLuint vao_ = 0, vbo_ = 0, ebo_ = 0;
    GLsizei count_ = 0;
};