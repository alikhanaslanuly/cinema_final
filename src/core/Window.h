#pragma once
#include <string>

struct GLFWwindow;

// GLFW window + OpenGL 3.3 core context + glad loading. Resize-safe: fbWidth()/fbHeight() are
// the current framebuffer size as of the last updateSize().
class Window {
public:
    Window() = default;
    ~Window();
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    bool init(int width, int height, const std::string& title);
    GLFWwindow* handle() const { return m_win; }
    bool shouldClose() const;
    void requestClose();
    void swapBuffers();
    static void pollEvents();
    void updateSize();  // call once per frame after pollEvents(): resize-safe framebuffer size

    int fbWidth() const { return m_fbW; }
    int fbHeight() const { return m_fbH; }
    float aspect() const { return m_fbH > 0 ? static_cast<float>(m_fbW) / static_cast<float>(m_fbH) : 1.0f; }
    void setTitle(const std::string& t);
    void setVsync(bool on);
    bool vsync() const { return m_vsync; }

private:
    GLFWwindow* m_win = nullptr;
    int m_fbW = 0, m_fbH = 0;
    bool m_vsync = true;
};
