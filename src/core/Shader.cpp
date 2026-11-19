#include "core/Shader.h"

#include <glm/gtc/type_ptr.hpp>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <vector>

#include "core/Paths.h"

namespace {

std::string dirOf(const std::string& p) {
    size_t i = p.find_last_of("/\\");
    return i == std::string::npos ? std::string() : p.substr(0, i + 1);
}

// Reads a shader file, expanding  #include "x"  recursively.
// 'files' collects every file in order: its index is the GLSL source-string number used in #line,
// so a driver message like "0(12)" / "1(7)" can be mapped back to a file.
bool readSource(const std::string& path, std::string& out, std::vector<std::string>& files, int depth) {
    if (depth > 8) {
        std::fprintf(stderr, "[Shader] #include nesting too deep at '%s'\n", path.c_str());
        return false;
    }
    std::ifstream f(path);
    if (!f) {
        std::fprintf(stderr, "[Shader] cannot open file '%s'\n", path.c_str());
        return false;
    }
    int myIndex = static_cast<int>(files.size());
    files.push_back(path);
    std::string line;
    int lineNo = 0;
    while (std::getline(f, line)) {
        ++lineNo;
        size_t p = line.find_first_not_of(" \t");
        if (p != std::string::npos && line.compare(p, 8, "#include") == 0) {
            size_t a = line.find('"', p), b = line.find('"', a == std::string::npos ? 0 : a + 1);
            if (a == std::string::npos || b == std::string::npos) {
                std::fprintf(stderr, "[Shader] bad #include at %s:%d\n", path.c_str(), lineNo);
                return false;
            }
            std::string inc = dirOf(path) + line.substr(a + 1, b - a - 1);
            int incIndex = static_cast<int>(files.size());
            out += "#line 1 " + std::to_string(incIndex) + "\n";
            if (!readSource(inc, out, files, depth + 1)) return false;
            out += "\n#line " + std::to_string(lineNo + 1) + " " + std::to_string(myIndex) + "\n";
        } else {
            out += line + "\n";
        }
    }
    return true;
}

void printFileMap(const std::vector<std::string>& files) {
    for (size_t i = 0; i < files.size(); ++i) std::fprintf(stderr, "    source %zu = %s\n", i, files[i].c_str());
}

GLuint compileStage(GLenum type, const std::string& path) {
    std::string src;
    std::vector<std::string> files;
    if (!readSource(path, src, files, 0)) return 0;
    GLuint s = glCreateShader(type);
    const char* p = src.c_str();
    glShaderSource(s, 1, &p, nullptr);
    glCompileShader(s);
    GLint ok = 0;
    glGetShaderiv(s, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        GLint len = 0;
        glGetShaderiv(s, GL_INFO_LOG_LENGTH, &len);
        std::vector<char> log(len > 1 ? len : 1);
        glGetShaderInfoLog(s, static_cast<GLsizei>(log.size()), nullptr, log.data());
        std::fprintf(stderr, "[Shader] COMPILE ERROR in '%s':\n%s\n", path.c_str(), log.data());
        printFileMap(files);
        glDeleteShader(s);
        return 0;
    }
    return s;
}

}  // namespace

Shader::~Shader() {
    if (m_id) glDeleteProgram(m_id);
}

GLuint Shader::build() const {
    GLuint vs = compileStage(GL_VERTEX_SHADER, m_vert);
    GLuint fs = compileStage(GL_FRAGMENT_SHADER, m_frag);
    if (!vs || !fs) {
        if (vs) glDeleteShader(vs);
        if (fs) glDeleteShader(fs);
        return 0;
    }
    GLuint prog = glCreateProgram();
    glAttachShader(prog, vs);
    glAttachShader(prog, fs);
    glLinkProgram(prog);
    glDeleteShader(vs);
    glDeleteShader(fs);
    GLint ok = 0;
    glGetProgramiv(prog, GL_LINK_STATUS, &ok);
    if (!ok) {
        GLint len = 0;
        glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
        std::vector<char> log(len > 1 ? len : 1);
        glGetProgramInfoLog(prog, static_cast<GLsizei>(log.size()), nullptr, log.data());
        std::fprintf(stderr, "[Shader] LINK ERROR (%s + %s):\n%s\n", m_vert.c_str(), m_frag.c_str(), log.data());
        glDeleteProgram(prog);
        return 0;
    }
    return prog;
}

bool Shader::load(const std::string& vertRel, const std::string& fragRel) {
    m_vert = paths::resolve(vertRel);
    m_frag = paths::resolve(fragRel);
    GLuint prog = build();
    if (!prog) return false;
    if (m_id) glDeleteProgram(m_id);
    m_id = prog;
    m_cache.clear();
    return true;
}

bool Shader::reload() {
    GLuint prog = build();
    if (!prog) {
        std::fprintf(stderr, "[Shader] reload failed, keeping the previous program\n");
        return false;
    }
    if (m_id) glDeleteProgram(m_id);
    m_id = prog;
    m_cache.clear();
    std::printf("[Shader] reloaded %s\n", m_frag.c_str());
    return true;
}

void Shader::use() const { glUseProgram(m_id); }

GLint Shader::loc(const char* name) const {
    auto it = m_cache.find(name);
    if (it != m_cache.end()) return it->second;
    GLint l = glGetUniformLocation(m_id, name);  // -1 (optimised out / not used): silently ignored by glUniform*
    m_cache.emplace(name, l);
    return l;
}

void Shader::setInt(const char* n, int v) const { glUniform1i(loc(n), v); }
void Shader::setFloat(const char* n, float v) const { glUniform1f(loc(n), v); }
void Shader::setVec2(const char* n, const glm::vec2& v) const { glUniform2f(loc(n), v.x, v.y); }
void Shader::setVec3(const char* n, const glm::vec3& v) const { glUniform3f(loc(n), v.x, v.y, v.z); }
void Shader::setMat4(const char* n, const glm::mat4& m) const { glUniformMatrix4fv(loc(n), 1, GL_FALSE, glm::value_ptr(m)); }
