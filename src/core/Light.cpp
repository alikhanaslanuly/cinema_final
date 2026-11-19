#include "core/Light.h"

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>
#include <cmath>

#include "core/Input.h"

void Light::configure(const glm::vec3& mn, const glm::vec3& mx) {
    m_center = (mn + mx) * 0.5f;
    m_target = glm::vec3(m_center.x, mn.y, m_center.z);  // look at the floor in the middle of the hall
    m_height = mx.y - 0.8f;                              // hangs just under the ceiling
    m_maxRadius = 0.4f * std::min(mx.x - mn.x, mx.z - mn.z);
    m_radius = std::min(m_radius, m_maxRadius);
}

void Light::update(float dt, const Input& in) {
    const float rotSpeed = 1.6f, moveSpeed = 3.0f;
    if (in.down(GLFW_KEY_LEFT)) m_azimuth -= rotSpeed * dt;
    if (in.down(GLFW_KEY_RIGHT)) m_azimuth += rotSpeed * dt;
    if (in.down(GLFW_KEY_UP)) m_radius += moveSpeed * dt;
    if (in.down(GLFW_KEY_DOWN)) m_radius -= moveSpeed * dt;
    m_radius = std::max(0.3f, std::min(m_radius, m_maxRadius));
}

glm::vec3 Light::position() const {
    return glm::vec3(m_center.x + m_radius * std::cos(m_azimuth), m_height, m_center.z + m_radius * std::sin(m_azimuth));
}

glm::vec3 Light::direction() const { return glm::normalize(m_target - position()); }

glm::mat4 Light::view() const {
    glm::vec3 d = direction();
    glm::vec3 up = std::fabs(d.y) > 0.99f ? glm::vec3(0.0f, 0.0f, 1.0f) : glm::vec3(0.0f, 1.0f, 0.0f);
    return glm::lookAt(position(), position() + d, up);
}

glm::mat4 Light::projection() const {
    return glm::perspective(glm::radians(fovDeg()), 1.0f, nearPlane, farPlane);
}
