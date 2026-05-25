#pragma once

#include "core/object3d.hpp"
#include "math/Vec3.hpp"
#include "math/Vec2.hpp"

namespace ygl {

struct Vertex {
    Vec3 position;
    Vec2 texcoord;
    Vec3 normal;
};

class Light : public Object3D {
public:
    enum class Type { DIRECTIONAL, POINT, SPOT };

    Light(Type type = Type::DIRECTIONAL);

    // Méthodes de Light
    Type getType() const;
    void setType(Type type);
    const Vec3& getColor() const;
    void setColor(const Vec3& color);
    float getIntensity() const;
    void setIntensity(float intensity);
    const Vec3& getDirection() const;
    void setDirection(const Vec3& direction);
    float getRange() const;
    void setRange(float range);

    // Surcharges de Object3D (pour éviter les warnings de hiding)
    const Vec3& getPosition() const;
    void setPosition(const Vec3& position);

private:
    Type m_type;
    Vec3 m_color;
    float m_intensity;
    Vec3 m_direction;
    float m_range;
};

} // namespace ygl
