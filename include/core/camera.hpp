#pragma once

#include "core/object3d.hpp"
#include "math/vec3.hpp"
#include "math/mat4.hpp"

namespace ygl {

class Camera : public Object3D {
public:
    enum class ProjectionType { PERSPECTIVE, ORTHOGRAPHIC };

    Camera(const Vec3& position = Vec3(0.0f, 0.0f, 5.0f),
           const Vec3& target = Vec3::zero(),
           const Vec3& up = Vec3::unitY());
    virtual ~Camera();

    void setProjectionType(ProjectionType type);
    ProjectionType getProjectionType() const;

    void setFov(float fov);
    float getFov() const;

    void setAspect(float aspect);
    float getAspect() const;

    void setNear(float near);
    float getNear() const;

    void setFar(float far);
    float getFar() const;

    void setOrthographicSize(float size);
    float getOrthographicSize() const;

    void setTarget(const Vec3& target);
    const Vec3& getTarget() const;

    void setUp(const Vec3& up);
    const Vec3& getUp() const;

    const Vec3& getForward() const;
    const Vec3& getRight() const;

    const mat4& getViewMatrix();  // Mat4 → mat4
    const mat4& getProjectionMatrix();  // Mat4 → mat4
    const mat4& getViewProjectionMatrix();  // Mat4 → mat4

    void moveForward(float distance);
    void moveBackward(float distance);
    void moveLeft(float distance);
    void moveRight(float distance);
    void moveUp(float distance);
    void moveDown(float distance);

    void rotateYaw(float angle);
    void rotatePitch(float angle);
    void rotateRoll(float angle);

    void processMouseMovement(double xpos, double ypos);
    void setMouseSensitivity(float sensitivity);
    float getMouseSensitivity() const;

    virtual void update(float deltaTime) override;

private:
    void updateViewMatrix() const;
    void updateProjectionMatrix() const;
    void updateViewProjectionMatrix() const;
    void updateVectors();

    ProjectionType m_projectionType = ProjectionType::PERSPECTIVE;
    float m_fov = 45.0f;
    float m_aspect = 16.0f / 9.0f;
    float m_near = 0.1f;
    float m_far = 1000.0f;
    float m_orthographicSize = 10.0f;

    Vec3 m_target;
    Vec3 m_up;
    Vec3 m_forward;
    Vec3 m_right;

    mutable mat4 m_viewMatrix;  // Mat4 → mat4
    mutable mat4 m_projectionMatrix;  // Mat4 → mat4
    mutable mat4 m_viewProjectionMatrix;  // Mat4 → mat4

    mutable bool m_viewDirty = true;
    mutable bool m_projectionDirty = true;
    mutable bool m_viewProjectionDirty = true;

    double m_lastMouseX = 0.0;
    double m_lastMouseY = 0.0;
    bool m_firstMouse = true;
    float m_yaw = -90.0f;
    float m_pitch = 0.0f;
    float m_mouseSensitivity = 0.1f;
};

} // namespace ygl
