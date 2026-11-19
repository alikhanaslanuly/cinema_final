#include "core/Camera.h"

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>
#include <cmath>

#include "core/Input.h"

namespace {
const float kPi = 3.14159265358979f;
float rad(float d) { return d * kPi / 180.0f; }
float deg(float r) { return r * 180.0f / kPi; }
}  // namespace

void Camera::anglesFromDir(const glm::vec3& d, float& yawDeg, float& pitchDeg) {
    glm::vec3 n = glm::normalize(d);
    yawDeg = deg(std::atan2(n.z, n.x));
    pitchDeg = deg(std::asin(std::max(-1.0f, std::min(1.0f, n.y))));
}

void Camera::updateFrontFromAngles() {
    float cy = std::cos(rad(m_yaw)), sy = std::sin(rad(m_yaw));
    float cp = std::cos(rad(m_pitch)), sp = std::sin(rad(m_pitch));
    m_front = glm::normalize(glm::vec3(cy * cp, sp, sy * cp));
}

void Camera::updateOrbitPose() {
    float cy = std::cos(rad(m_oYaw)), sy = std::sin(rad(m_oYaw));
    float cp = std::cos(rad(m_oPitch)), sp = std::sin(rad(m_oPitch));
    glm::vec3 dir(cy * cp, sp, sy * cp);
    m_eye = m_target + m_radius * dir;
    m_front = glm::normalize(m_target - m_eye);
}

void Camera::snapFirstPerson(const glm::vec3& eye, const glm::vec3& lookAt) {
    m_trans.active = false;
    m_mode = Mode::FirstPerson;
    m_eye = eye;
    anglesFromDir(lookAt - eye, m_yaw, m_pitch);
    updateFrontFromAngles();
}

void Camera::setFirstPerson(const glm::vec3& eye, const glm::vec3& lookAt) {
    float curYaw, curPitch;
    anglesFromDir(m_front, curYaw, curPitch);  // works for both modes (m_front is always valid)
    float yaw1, pitch1;
    anglesFromDir(lookAt - eye, yaw1, pitch1);
    m_mode = Mode::FirstPerson;
    m_yaw = curYaw;
    m_pitch = curPitch;
    m_trans.active = true;
    m_trans.t = 0.0f;
    m_trans.eye0 = m_eye;
    m_trans.eye1 = eye;
    m_trans.yaw0 = curYaw;
    float d = std::fmod(yaw1 - curYaw + 540.0f, 360.0f) - 180.0f;  // shortest way round
    m_trans.dyaw = d;
    m_trans.pitch0 = curPitch;
    m_trans.pitch1 = pitch1;
}

void Camera::setOrbit(const glm::vec3& target, float radius, float yawDeg, float pitchDeg) {
    m_trans.active = false;
    m_mode = Mode::Orbit;
    m_target = target;
    m_radius = radius;
    m_oYaw = yawDeg;
    m_oPitch = pitchDeg;
    updateOrbitPose();
}

void Camera::toggleOrbit(const glm::vec3& target, float radius) {
    if (m_mode == Mode::FirstPerson) {
        m_walkEye = m_eye;
        m_walkYaw = m_yaw;
        m_walkPitch = m_pitch;
        setOrbit(target, radius, 35.0f, 28.0f);
    } else {
        m_trans.active = false;
        m_mode = Mode::FirstPerson;
        m_eye = m_walkEye;
        m_yaw = m_walkYaw;
        m_pitch = m_walkPitch;
        updateFrontFromAngles();
    }
}

void Camera::update(float dt, const Input& in, bool controls, bool autoOrbit) {
    if (m_trans.active) {
        m_trans.t += dt / m_trans.duration;
        float t = std::min(1.0f, m_trans.t);
        float s = t * t * (3.0f - 2.0f * t);  // smoothstep
        m_eye = m_trans.eye0 + (m_trans.eye1 - m_trans.eye0) * s;
        m_yaw = m_trans.yaw0 + m_trans.dyaw * s;
        m_pitch = m_trans.pitch0 + (m_trans.pitch1 - m_trans.pitch0) * s;
        updateFrontFromAngles();
        if (m_trans.t >= 1.0f) m_trans.active = false;
        return;
    }

    if (m_mode == Mode::FirstPerson) {
        if (!controls) return;
        if (in.cursorCaptured()) {
            glm::vec2 md = in.mouseDelta();
            m_yaw += md.x * mouseSens;
            m_pitch = std::max(-89.0f, std::min(89.0f, m_pitch - md.y * mouseSens));
            updateFrontFromAngles();
        }
        glm::vec3 fwd = glm::normalize(glm::vec3(m_front.x, 0.0f, m_front.z));
        glm::vec3 right = glm::normalize(glm::cross(fwd, glm::vec3(0.0f, 1.0f, 0.0f)));
        glm::vec3 move(0.0f);
        if (in.down(GLFW_KEY_W)) move += fwd;
        if (in.down(GLFW_KEY_S)) move -= fwd;
        if (in.down(GLFW_KEY_D)) move += right;
        if (in.down(GLFW_KEY_A)) move -= right;
        float speed = walkSpeed * (in.down(GLFW_KEY_LEFT_SHIFT) ? 2.0f : 1.0f);
        if (glm::length(move) > 0.0f) m_eye += glm::normalize(move) * speed * dt;
        const float margin = 0.5f;
        m_eye.x = std::max(m_bMin.x + margin, std::min(m_eye.x, m_bMax.x - margin));
        m_eye.z = std::max(m_bMin.z + margin, std::min(m_eye.z, m_bMax.z - margin));
        float floorY = m_floor ? m_floor(m_eye.x, m_eye.z) : 0.0f;
        float wantY = floorY + eyeHeight;
        m_eye.y += (wantY - m_eye.y) * std::min(1.0f, 12.0f * dt);  // smooth steps between tiers
    } else {
        if (autoOrbit) {
            m_oYaw += 10.0f * dt;
        } else if (controls) {
            if (in.mouseDown(GLFW_MOUSE_BUTTON_RIGHT)) {
                glm::vec2 md = in.mouseDelta();
                m_oYaw += md.x * 0.3f;
                m_oPitch = std::max(5.0f, std::min(85.0f, m_oPitch + md.y * 0.3f));
            }
            m_radius = std::max(4.0f, std::min(40.0f, m_radius - in.scroll() * 1.0f));
        }
        updateOrbitPose();
    }
}

glm::mat4 Camera::view() const { return glm::lookAt(m_eye, m_eye + m_front, glm::vec3(0.0f, 1.0f, 0.0f)); }

glm::mat4 Camera::projection(float aspect) const {
    return glm::perspective(glm::radians(fovDeg), aspect, nearPlane, farPlane);
}

Ray Camera::centerRay() const {
    Ray r;
    r.origin = m_eye;
    r.dir = glm::normalize(m_front);
    return r;
}
