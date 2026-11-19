#include "core/Input.h"

#include <GLFW/glfw3.h>

#include <cstring>

namespace {
Input* g_self = nullptr;  // GLFW callbacks are plain functions
}

void Input::attach(GLFWwindow* window) {
    m_win = window;
    g_self = this;
    glfwSetKeyCallback(window, keyCb);
    glfwSetMouseButtonCallback(window, mouseCb);
    glfwSetCursorPosCallback(window, cursorCb);
    glfwSetScrollCallback(window, scrollCb);
}

void Input::newFrame() {
    std::memset(m_pressed, 0, sizeof(m_pressed));
    std::memset(m_mPressed, 0, sizeof(m_mPressed));
    m_delta = glm::vec2(0.0f);
    m_scroll = 0.0f;
}

bool Input::down(int k) const { return k >= 0 && k < 512 && m_down[k]; }
bool Input::pressed(int k) const { return k >= 0 && k < 512 && m_pressed[k]; }
bool Input::mouseDown(int b) const { return b >= 0 && b < 8 && m_mDown[b]; }
bool Input::mousePressed(int b) const { return b >= 0 && b < 8 && m_mPressed[b]; }

void Input::setCursorCaptured(bool captured) {
    if (captured == m_captured) return;
    m_captured = captured;
    glfwSetInputMode(m_win, GLFW_CURSOR, captured ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
    if (captured && glfwRawMouseMotionSupported()) glfwSetInputMode(m_win, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    m_haveLast = false;  // avoid a camera jump on the first event after the mode change
}

void Input::keyCb(GLFWwindow*, int key, int, int action, int) {
    if (!g_self || key < 0 || key >= 512) return;
    if (action == GLFW_PRESS) {
        g_self->m_down[key] = true;
        g_self->m_pressed[key] = true;
    } else if (action == GLFW_RELEASE) {
        g_self->m_down[key] = false;
    }
}

void Input::mouseCb(GLFWwindow*, int button, int action, int) {
    if (!g_self || button < 0 || button >= 8) return;
    if (action == GLFW_PRESS) {
        g_self->m_mDown[button] = true;
        g_self->m_mPressed[button] = true;
    } else if (action == GLFW_RELEASE) {
        g_self->m_mDown[button] = false;
    }
}

void Input::cursorCb(GLFWwindow*, double x, double y) {
    if (!g_self) return;
    glm::vec2 p(static_cast<float>(x), static_cast<float>(y));
    if (g_self->m_haveLast) g_self->m_delta += p - g_self->m_last;
    g_self->m_last = p;
    g_self->m_haveLast = true;
}

void Input::scrollCb(GLFWwindow*, double, double dy) {
    if (g_self) g_self->m_scroll += static_cast<float>(dy);
}
