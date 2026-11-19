#pragma once
#include <glad/glad.h>

// Depth-only framebuffer for the shadow pass (pass 1 of shadow mapping).
//   begin()  -> bind FBO, set viewport to res x res, clear depth
//   ...draw the scene with shaders/core/depth.* ...
//   end()    -> restore the default framebuffer and viewport
// The depth texture is then sampled in shaders/core/shadow.glsl (texture unit chosen by bindTexture).
class ShadowMap {
public:
    static constexpr int kLow = 512;
    static constexpr int kHigh = 2048;

    ShadowMap() = default;
    ~ShadowMap();
    ShadowMap(const ShadowMap&) = delete;
    ShadowMap& operator=(const ShadowMap&) = delete;

    bool init(int resolution);
    bool resize(int resolution);  // recreates texture + FBO
    void toggleQuality();         // Q: 512 <-> 2048

    void begin();
    void end(int fbWidth, int fbHeight);
    void bindTexture(int unit) const;

    bool ok() const { return m_ok; }
    int resolution() const { return m_res; }
    GLuint texture() const { return m_tex; }

    // Depth-pass face culling: false = back faces culled (default), true = front faces culled
    // (alternative anti-acne trick, key C). Floors/walls of one-sided quads need false.
    bool cullFrontFaces = false;

private:
    void destroy();
    GLuint m_fbo = 0, m_tex = 0;
    int m_res = 0;
    bool m_ok = false;
    GLint m_prevFbo = 0;
};
