#pragma once
struct GLFWwindow;

// Dear ImGui setup (GLFW + OpenGL3 backends) with a Cyrillic-capable font.
// Frame: beginFrame() -> ui draws widgets -> render() after the 3D scene.
class ImGuiLayer {
public:
    bool init(GLFWwindow* window);
    void shutdown();
    void beginFrame();
    void render();
    bool wantsMouse() const;  // true while the cursor is over an ImGui window
private:
    bool m_ready = false;
};
