#pragma once

#include "math/vec3.hpp"
#include "math/quat.hpp"
#include "math/mat4.hpp"
#include <vector>
#include <memory>
#include <string>

namespace ygl {

class Object3D : public std::enable_shared_from_this<Object3D> {
public:
    Object3D(const std::string& name = "");
    virtual ~Object3D();

    void setPosition(const Vec3& position);
    void setRotation(const Quat& rotation);
    void setScale(const Vec3& scale);

    void translate(const Vec3& translation);
    void rotate(const Quat& rotation);
    void scale(const Vec3& scale);

    const Vec3& getPosition() const { return m_position; }
    const Quat& getRotation() const { return m_rotation; }
    const Vec3& getScale() const { return m_scale; }

    const Vec3& getWorldPosition() const { return m_worldPosition; }
    const Quat& getWorldRotation() const { return m_worldRotation; }
    const Vec3& getWorldScale() const { return m_worldScale; }

    const Mat4& getLocalMatrix() const;
    const Mat4& getWorldMatrix() const;
    const Mat4& getNormalMatrix() const;

    void updateMatrix(bool force = false) const;
    void updateWorldMatrix(bool force = false) const;

    void addChild(std::shared_ptr<Object3D> child);
    void removeChild(std::shared_ptr<Object3D> child);
    void removeChild(size_t index);
    void clearChildren();

    std::shared_ptr<Object3D> getChild(size_t index) const;
    size_t getChildCount() const { return m_children.size(); }

    void setParent(std::shared_ptr<Object3D> parent);
    std::shared_ptr<Object3D> getParent() const;

    const std::string& getName() const { return m_name; }
    void setName(const std::string& name) { m_name = name; }

    bool isVisible() const { return m_visible; }
    void setVisible(bool visible) { m_visible = visible; }

    virtual void update(float deltaTime);
    virtual void render() {}

protected:
    std::string m_name;
    Vec3 m_position;
    Quat m_rotation;
    Vec3 m_scale;

    Vec3 m_worldPosition;
    Quat m_worldRotation;
    Vec3 m_worldScale;

    mutable Mat4 m_localMatrix;
    mutable Mat4 m_worldMatrix;
    mutable Mat4 m_normalMatrix;

    mutable bool m_localMatrixDirty = true;
    mutable bool m_worldMatrixDirty = true;
    mutable bool m_normalMatrixDirty = true;

    std::vector<std::shared_ptr<Object3D>> m_children;
    std::weak_ptr<Object3D> m_parent;

    bool m_visible = true;
};

} // namespace ygl
