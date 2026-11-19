#pragma once
#include <glm/glm.hpp>

class Input;

// Movable ceiling spot light that casts the shadows.
//   Left/Right arrows: rotate the lamp around the hall axis (azimuth)
//   Up/Down arrows   : move it towards / away from the hall centre
// The light looks at the hall centre; the shadow map is a perspective projection with the same
// cone (so everything lit is also inside the shadow frustum).
class Light {
public:
    void configure(const glm::vec3& hallMin, const glm::vec3& hallMax);
    void update(float dt, const Input& in);

    glm::vec3 position() const;
    glm::vec3 direction() const;  // unit vector from the lamp towards its target
    glm::mat4 view() const;
    glm::mat4 projection() const;
    glm::mat4 lightSpace() const { return projection() * view(); }

    float nearPlane = 0.3f, farPlane = 60.0f;
    float innerDeg = 40.0f, outerDeg = 60.0f;  // spot cone (full intensity / zero intensity)
    glm::vec3 color{3.0f, 2.8f, 2.4f};         // warm white, intensity baked in
    float fovDeg() const { return 2.0f * outerDeg + 6.0f; }

private:
    glm::vec3 m_center{0.0f}, m_target{0.0f};
    float m_height = 6.0f;
    float m_azimuth = 0.6f;  // rad
    float m_radius = 2.5f, m_maxRadius = 6.0f;
};
