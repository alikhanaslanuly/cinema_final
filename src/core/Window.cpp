#include "core/Window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cstdio>

namespace {
void onGlfwError(int code, const char* msg) { std::fprintf(stderr, "[GLFW] error %d: %s\n", code, msg); }
}  // namespace

Window::~Window() {
    if (m_win) glfwDestroyWindow(m_win);
    glfwTerminate();
}

bool Window::init(int width, int height, const std::string& title) {
    glfwSetErrorCallback(onGlfwError);
    if (!glfwInit()) {
        std::fprintf(stderr, "[Window] glfwInit failed\n");
        return false;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    m_win = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!m_win) {
        std::fprintf(stderr, "[Window] cannot create an OpenGL 3.3 core window (update the GPU driver)\n");
        return false;
    }
    glfwMakeContextCurrent(m_win);
    glfwSetWindowUserPointer(m_win, this);
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::fprintf(stderr, "[Window] failed to load OpenGL functions\n");
        return false;
    }
    setVsync(true);
    glfwGetFramebufferSize(m_win, &m_fbW, &m_fbH);
    std::printf("[GL] %s | %s | GLSL %s\n", glGetString(GL_RENDERER), glGetString(GL_VERSION),
                glGetString(GL_SHADING_LANGUAGE_VERSION));
    GLint major = 0, minor = 0;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    if (major < 3 || (major == 3 && minor < 3)) {
        std::fprintf(stderr, "[Window] OpenGL 3.3 is required, got %d.%d\n", major, minor);
        return false;
    }
    return true;
}

bool Window::shouldClose() const { return glfwWindowShouldClose(m_win) != 0; }
void Window::requestClose() { glfwSetWindowShouldClose(m_win, GLFW_TRUE); }
void Window::swapBuffers() { glfwSwapBuffers(m_win); }
void Window::updateSize() { glfwGetFramebufferSize(m_win, &m_fbW, &m_fbH); }
void Window::pollEvents() { glfwPollEvents(); }
void Window::setTitle(const std::string& t) { glfwSetWindowTitle(m_win, t.c_str()); }
void Window::setVsync(bool on) {
    m_vsync = on;
    glfwSwapInterval(on ? 1 : 0);
}
