#include "core/object3d.hpp"
#include <algorithm>

namespace ygl {

Object3D::Object3D(const std::string& name)
    : m_name(name), m_position(Vec3::zero()),
      m_rotation(Quat::identity()), m_scale(Vec3::one()),
      m_worldPosition(Vec3::zero()), m_worldRotation(Quat::identity()),
      m_worldScale(Vec3::one()) {}

Object3D::~Object3D() {
    clearChildren();
}

// Transform
void Object3D::setPosition(const Vec3& position) {
    m_position = position; m_localMatrixDirty = true; m_worldMatrixDirty = true;
}
void Object3D::setRotation(const Quat& rotation) {
    m_rotation = rotation; m_localMatrixDirty = true; m_worldMatrixDirty = true;
}
void Object3D::setScale(const Vec3& scale) {
    m_scale = scale; m_localMatrixDirty = true; m_worldMatrixDirty = true;
}

void Object3D::translate(const Vec3& translation) {
    m_position += translation; m_localMatrixDirty = true; m_worldMatrixDirty = true;
}
void Object3D::rotate(const Quat& rotation) {
    m_rotation = rotation * m_rotation; m_localMatrixDirty = true; m_worldMatrixDirty = true;
}
void Object3D::scale(const Vec3& scale) {
    m_scale = m_scale * scale; m_localMatrixDirty = true; m_worldMatrixDirty = true;
}

// Matrix
void Object3D::updateMatrix(bool force) const {
    if (!force && !m_localMatrixDirty) return;
    mat4 translation = mat4::Translation(m_position);
    mat4 rotation = m_rotation.toMatrix();
    mat4 scale = mat4::Scale(m_scale);
    m_localMatrix = translation * rotation * scale;
    m_localMatrixDirty = false;
}

void Object3D::updateWorldMatrix(bool force) const {
    if (!force && !m_worldMatrixDirty) return;
    updateMatrix();
    auto parent = m_parent.lock();
    if (parent) {
        parent->updateWorldMatrix();
        m_worldMatrix = parent->getWorldMatrix() * m_localMatrix;
    } else {
        m_worldMatrix = m_localMatrix;
    }
    // Utilisation de const_cast pour modifier les membres mutable
    const_cast<Vec3&>(m_worldPosition) = Vec3(m_worldMatrix(0,3), m_worldMatrix(1,3), m_worldMatrix(2,3));
    const_cast<Vec3&>(m_worldScale) = Vec3(
        Vec3(m_worldMatrix(0,0), m_worldMatrix(1,0), m_worldMatrix(2,0)).Length(),
        Vec3(m_worldMatrix(0,1), m_worldMatrix(1,1), m_worldMatrix(2,1)).Length(),
        Vec3(m_worldMatrix(0,2), m_worldMatrix(1,2), m_worldMatrix(2,2)).Length()
    );
    const_cast<Quat&>(m_worldRotation) = Quat(m_worldMatrix);
    m_worldMatrixDirty = false;
    m_normalMatrixDirty = true;
}

const mat4& Object3D::getLocalMatrix() const { updateMatrix(); return m_localMatrix; }
const mat4& Object3D::getWorldMatrix() const { updateWorldMatrix(); return m_worldMatrix; }

const mat4& Object3D::getNormalMatrix() const {
    if (m_normalMatrixDirty) {
        updateWorldMatrix();
        m_normalMatrix = m_worldMatrix.inverted().transposed();
        m_normalMatrixDirty = false;
    }
    return m_normalMatrix;
}

// Hierarchy
void Object3D::addChild(std::shared_ptr<Object3D> child) {
    if (!child) return;
    auto currentParent = child->getParent();
    if (currentParent) currentParent->removeChild(child);
    child->setParent(shared_from_this());
    m_children.push_back(child);
    m_worldMatrixDirty = true;
}

void Object3D::removeChild(std::shared_ptr<Object3D> child) {
    for (auto it = m_children.begin(); it != m_children.end(); ++it) {
        if (*it == child) {
            (*it)->setParent(nullptr);
            m_children.erase(it);
            m_worldMatrixDirty = true;
            return;
        }
    }
}

void Object3D::removeChild(size_t index) {
    if (index < m_children.size()) {
        m_children[index]->setParent(nullptr);
        m_children.erase(m_children.begin() + index);
        m_worldMatrixDirty = true;
    }
}

void Object3D::clearChildren() {
    for (auto& child : m_children) child->setParent(nullptr);
    m_children.clear();
    m_worldMatrixDirty = true;
}

std::shared_ptr<Object3D> Object3D::getChild(size_t index) const {
    return index < m_children.size() ? m_children[index] : nullptr;
}

void Object3D::setParent(std::shared_ptr<Object3D> parent) {
    m_parent = parent;
    m_worldMatrixDirty = true;
}

std::shared_ptr<Object3D> Object3D::getParent() const {
    return m_parent.lock();
}

// Update
void Object3D::update(float deltaTime) {
    m_localMatrixDirty = true;
    m_worldMatrixDirty = true;
    m_normalMatrixDirty = true;
    for (auto& child : m_children) child->update(deltaTime);
}

} // namespace ygl
