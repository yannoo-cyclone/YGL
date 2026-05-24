#pragma once

#include "math/vec3.hpp"

namespace ygl {

class Light {
public:
    enum class Type { DIRECTIONAL, POINT, SPOT };

    Light(Type type = Type::DIRECTIONAL);

    Type getType() const;
    void setType(Type type);

    const Vec3& getColor() const;
    void setColor(const Vec3& color);

    float getIntensity() const;
    void setIntensity(float intensity);

    const Vec3& getDirection() const;
    void setDirection(const Vec3& direction);

    const Vec3& getPosition() const;
    void setPosition(const Vec3& position);

    float getRange() const;
    void setRange(float range);

private:
    Type m_type;
    Vec3 m_color;
    float m_intensity;
    Vec3 m_direction;
    Vec3 m_position;
    float m_range;
};

} // namespace ygl
