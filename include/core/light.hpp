#pragma once

#include "core/object3d.hpp"  // AJOUTÉ : Light hérite de Object3D
#include "math/vec3.hpp"
#include <string>

namespace ygl {

// Déclaration de la struct Vertex (si elle n'est pas définie ailleurs)
struct Vertex {
    Vec3 position;
    Vec2 texcoord;
    Vec3 normal;
};

class Light : public Object3D {  // CORRIGÉ : hérite de Object3D
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
    float m_range;
};

} // namespace ygl
