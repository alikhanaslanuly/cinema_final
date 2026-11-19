#pragma once
#include <glm/glm.hpp>

struct GLFWwindow;

// Keyboard / mouse state. Call newFrame() BEFORE glfwPollEvents() every frame.
// pressed() = went down this frame (no key repeat); down() = currently held.
class Input {
public:
    void attach(GLFWwindow* window);  // install callbacks (before ImGui, which chains them)
    void newFrame();

    bool down(int key) const;
    bool pressed(int key) const;
    bool mouseDown(int button) const;
    bool mousePressed(int button) const;
    glm::vec2 mouseDelta() const { return m_delta; }
    float scroll() const { return m_scroll; }

    void setCursorCaptured(bool captured);  // true: hidden + endless mouse look (HALL)
    bool cursorCaptured() const { return m_captured; }

private:
    static void keyCb(GLFWwindow*, int key, int scancode, int action, int mods);
    static void mouseCb(GLFWwindow*, int button, int action, int mods);
    static void cursorCb(GLFWwindow*, double x, double y);
    static void scrollCb(GLFWwindow*, double dx, double dy);

    GLFWwindow* m_win = nullptr;
    bool m_down[512] = {};
    bool m_pressed[512] = {};
    bool m_mDown[8] = {};
    bool m_mPressed[8] = {};
    glm::vec2 m_delta{0.0f};
    glm::vec2 m_last{0.0f};
    bool m_haveLast = false;
    float m_scroll = 0.0f;
    bool m_captured = false;
};
