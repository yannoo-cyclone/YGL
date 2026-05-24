#include "core/camera.hpp"
#include <cmath>

namespace ygl {

Camera::Camera(const Vec3& position, const Vec3& target, const Vec3& up)
    : Object3D("Camera"), m_target(target), m_up(up) {
    setPosition(position);
    updateVectors();
    m_viewDirty = true;
    m_projectionDirty = true;
}

Camera::~Camera() {}

// Projection
void Camera::setProjectionType(ProjectionType type) {
    m_projectionType = type; m_projectionDirty = true;
}
Camera::ProjectionType Camera::getProjectionType() const { return m_projectionType; }

void Camera::setFov(float fov) { m_fov = fov; m_projectionDirty = true; }
float Camera::getFov() const { return m_fov; }

void Camera::setAspect(float aspect) { m_aspect = aspect; m_projectionDirty = true; }
float Camera::getAspect() const { return m_aspect; }

void Camera::setNear(float near) { m_near = near; m_projectionDirty = true; }
float Camera::getNear() const { return m_near; }

void Camera::setFar(float far) { m_far = far; m_projectionDirty = true; }
float Camera::getFar() const { return m_far; }

void Camera::setOrthographicSize(float size) { m_orthographicSize = size; m_projectionDirty = true; }
float Camera::getOrthographicSize() const { return m_orthographicSize; }

// View
void Camera::setTarget(const Vec3& target) { m_target = target; m_viewDirty = true; }
const Vec3& Camera::getTarget() const { return m_target; }

void Camera::setUp(const Vec3& up) { m_up = up; m_viewDirty = true; }
const Vec3& Camera::getUp() const { return m_up; }

const Vec3& Camera::getForward() const { return m_forward; }
const Vec3& Camera::getRight() const { return m_right; }

// Matrices
const mat4& Camera::getViewMatrix() {
    if (m_viewDirty) updateViewMatrix();
    return m_viewMatrix;
}

const mat4& Camera::getProjectionMatrix() {
    if (m_projectionDirty) updateProjectionMatrix();
    return m_projectionMatrix;
}

const mat4& Camera::getViewProjectionMatrix() {
    if (m_viewProjectionDirty) updateViewProjectionMatrix();
    return m_viewProjectionMatrix;
}

void Camera::updateViewMatrix() const {
    Vec3 pos = getWorldPosition();
    m_viewMatrix = mat4::lookAt(pos, m_target, m_up);
    m_viewDirty = false;
    m_viewProjectionDirty = true;
}

void Camera::updateProjectionMatrix() const {
    if (m_projectionType == ProjectionType::PERSPECTIVE) {
        m_projectionMatrix = mat4::perspective(
            m_fov * 3.14159265359f / 180.0f, m_aspect, m_near, m_far);
    } else {
        float halfSize = m_orthographicSize * 0.5f;
        m_projectionMatrix = mat4::orthographic(
            -halfSize * m_aspect, halfSize * m_aspect,
            -halfSize, halfSize, m_near, m_far);
    }
    m_projectionDirty = false;
    m_viewProjectionDirty = true;
}

void Camera::updateViewProjectionMatrix() const {
    updateViewMatrix(); updateProjectionMatrix();
    m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
    m_viewProjectionDirty = false;
}

void Camera::updateVectors() {
    Vec3 pos = getWorldPosition();
    m_forward = ygl::normalize(m_target - pos);
    m_right = ygl::normalize(ygl::cross(m_forward, m_up));
    m_up = ygl::cross(m_right, m_forward);
}

// Movement
void Camera::moveForward(float distance) {
    Vec3 pos = getWorldPosition();
    setPosition(pos + m_forward * distance);
    m_target = pos + m_forward * distance + m_forward;
    m_viewDirty = true;
}
void Camera::moveBackward(float distance) {
    Vec3 pos = getWorldPosition();
    setPosition(pos - m_forward * distance);
    m_target = pos - m_forward * distance + m_forward;
    m_viewDirty = true;
}
void Camera::moveLeft(float distance) {
    Vec3 pos = getWorldPosition();
    setPosition(pos - m_right * distance);
    m_target = pos - m_right * distance + m_forward;
    m_viewDirty = true;
}
void Camera::moveRight(float distance) {
    Vec3 pos = getWorldPosition();
    setPosition(pos + m_right * distance);
    m_target = pos + m_right * distance + m_forward;
    m_viewDirty = true;
}
void Camera::moveUp(float distance) {
    Vec3 pos = getWorldPosition();
    setPosition(pos + m_up * distance);
    m_target = pos + m_up * distance + m_forward;
    m_viewDirty = true;
}
void Camera::moveDown(float distance) {
    Vec3 pos = getWorldPosition();
    setPosition(pos - m_up * distance);
    m_target = pos - m_up * distance + m_forward;
    m_viewDirty = true;
}

// Rotation
void Camera::rotateYaw(float angle) {
    m_yaw += angle; updateVectors(); m_viewDirty = true;
}
void Camera::rotatePitch(float angle) {
    m_pitch += angle;
    if (m_pitch > 89.0f) m_pitch = 89.0f;
    if (m_pitch < -89.0f) m_pitch = -89.0f;
    updateVectors(); m_viewDirty = true;
}
void Camera::rotateRoll(float angle) {
    Quat rollRot(m_forward, angle);
    m_up = rollRot * m_up;
    m_right = rollRot * m_right;
    m_viewDirty = true;
}

// Mouse
void Camera::processMouseMovement(double xpos, double ypos) {
    if (m_firstMouse) {
        m_lastMouseX = xpos; m_lastMouseY = ypos; m_firstMouse = false;
    }
    float xoffset = static_cast<float>(xpos - m_lastMouseX);
    float yoffset = static_cast<float>(m_lastMouseY - ypos);
    m_lastMouseX = xpos; m_lastMouseY = ypos;
    xoffset *= m_mouseSensitivity; yoffset *= m_mouseSensitivity;
    rotateYaw(xoffset); rotatePitch(yoffset);
}

void Camera::setMouseSensitivity(float sensitivity) { m_mouseSensitivity = sensitivity; }
float Camera::getMouseSensitivity() const { return m_mouseSensitivity; }

// Update
void Camera::update(float deltaTime) {
    Object3D::update(deltaTime);
    Vec3 pos = getWorldPosition();
    Vec3 dir = m_target - pos;
    if (dir.lengthSquared() > 0.0001f) {
        m_forward = ygl::normalize(dir);
        m_right = ygl::normalize(ygl::cross(m_forward, m_up));
        m_up = ygl::cross(m_right, m_forward);
    }
    m_viewDirty = true;
}

} // namespace ygl
