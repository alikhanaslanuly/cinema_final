#pragma once
#include <functional>
#include <glm/glm.hpp>

#include "picking/Ray.h"

class Input;

// Perspective camera with two modes:
//   FirstPerson: WASD + mouse look, eye follows the floor height (tiers), clamped to the hall.
//   Orbit      : overview of the hall (RMB drag = rotate, wheel = zoom, or auto-rotate in the menu).
// setFirstPerson() glides smoothly to a new pose (walk -> seat), snapFirstPerson() jumps.
class Camera {
public:
    enum class Mode { FirstPerson, Orbit };

    void setWalkBounds(const glm::vec3& mn, const glm::vec3& mx) { m_bMin = mn; m_bMax = mx; }
    void setFloorHeightFn(std::function<float(float, float)> fn) { m_floor = std::move(fn); }

    // Contract: Camera::setFirstPerson(eyePos, lookAt). Smooth transition (~0.9 s).
    void setFirstPerson(const glm::vec3& eyePos, const glm::vec3& lookAt);
    void snapFirstPerson(const glm::vec3& eyePos, const glm::vec3& lookAt);
    void setOrbit(const glm::vec3& target, float radius, float yawDeg, float pitchDeg);
    void toggleOrbit(const glm::vec3& target, float radius);  // key O: walk <-> overview

    void update(float dt, const Input& in, bool controlsEnabled, bool autoOrbit);

    glm::mat4 view() const;
    glm::mat4 projection(float aspect) const;
    Ray centerRay() const;  // contract: ray from the crosshair (screen centre)

    glm::vec3 position() const { return m_eye; }
    glm::vec3 front() const { return m_front; }
    Mode mode() const { return m_mode; }
    bool transitioning() const { return m_trans.active; }

    float fovDeg = 60.0f, nearPlane = 0.1f, farPlane = 100.0f;
    float eyeHeight = 1.7f, walkSpeed = 3.2f, mouseSens = 0.10f;

private:
    struct Transition {
        bool active = false;
        float t = 0.0f, duration = 0.9f;
        glm::vec3 eye0{0.0f}, eye1{0.0f};
        float yaw0 = 0, dyaw = 0, pitch0 = 0, pitch1 = 0;
    };
    void updateFrontFromAngles();
    void updateOrbitPose();
    static void anglesFromDir(const glm::vec3& d, float& yawDeg, float& pitchDeg);

    Mode m_mode = Mode::FirstPerson;
    glm::vec3 m_eye{0.0f, 1.7f, 0.0f}, m_front{0.0f, 0.0f, -1.0f};
    float m_yaw = -90.0f, m_pitch = 0.0f;  // degrees, first person
    // orbit
    glm::vec3 m_target{0.0f};
    float m_radius = 15.0f, m_oYaw = 35.0f, m_oPitch = 25.0f;
    // saved walking pose (restored when leaving orbit)
    glm::vec3 m_walkEye{0.0f};
    float m_walkYaw = -90.0f, m_walkPitch = 0.0f;

    glm::vec3 m_bMin{-1e9f}, m_bMax{1e9f};
    std::function<float(float, float)> m_floor;
    Transition m_trans;
};
