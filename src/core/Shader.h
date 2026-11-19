#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

// GLSL program loaded from files. Supports  #include "file.glsl"  (path relative to the
// including file). All compile / link errors are printed to the console with file names.
class Shader {
public:
    Shader() = default;
    ~Shader();
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    bool load(const std::string& vertRel, const std::string& fragRel);
    bool reload();  // hot reload (F6); keeps the old program if the new one fails
    bool valid() const { return m_id != 0; }
    GLuint id() const { return m_id; }
    void use() const;

    void setInt(const char* name, int v) const;
    void setBool(const char* name, bool v) const { setInt(name, v ? 1 : 0); }
    void setFloat(const char* name, float v) const;
    void setVec2(const char* name, const glm::vec2& v) const;
    void setVec3(const char* name, const glm::vec3& v) const;
    void setMat4(const char* name, const glm::mat4& m) const;

private:
    GLint loc(const char* name) const;
    GLuint build() const;
    std::string m_vert, m_frag;
    GLuint m_id = 0;
    mutable std::unordered_map<std::string, GLint> m_cache;
};
