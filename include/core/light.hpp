#pragma once

#include "math/vec3.hpp"

namespace ygl {

class Light {
public:
    enum class Type { DIRECTIONAL, POINT, SPOT };

    Light(Type type = Type::DIRECTIONAL)
        : m_type(type), m_color(1.0f, 1.0f, 1.0f), m_intensity(1.0f) {}

    Type getType() const { return m_type; }
    void setType(Type type) { m_type = type; }

    const Vec3& getColor() const { return m_color; }
    void setColor(const Vec3& color) { m_color = color; }

    float getIntensity() const { return m_intensity; }
    void setIntensity(float intensity) { m_intensity = intensity; }

    const Vec3& getDirection() const { return m_direction; }
    void setDirection(const Vec3& direction) { m_direction = direction.Normalize(); }

    const Vec3& getPosition() const { return m_position; }
    void setPosition(const Vec3& position) { m_position = position; }

    float getRange() const { return m_range; }
    void setRange(float range) { m_range = range; }

private:
    Type m_type;
    Vec3 m_color;
    float m_intensity;
    Vec3 m_direction = Vec3(0.0f, -1.0f, 0.0f);
    Vec3 m_position;
    float m_range = 10.0f;
};

} // namespace ygl
