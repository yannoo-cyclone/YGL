#pragma once

#include "core/object3d.hpp"
#include "math/vec3.hpp"
#include "math/vec2.hpp"  // AJOUTÉ pour Vec2

namespace ygl {

// Déclaration de la struct Vertex (si elle n'est pas définie ailleurs)
struct Vertex {
    Vec3 position;
    Vec2 texcoord;
    Vec3 normal;
};

class Light : public Object3D {
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

    // m_position et m_scale sont hérités de Object3D → pas besoin de les redéclarer
    float getRange() const;
    void setRange(float range);

private:
    Type m_type;
    Vec3 m_color;
    float m_intensity;
    Vec3 m_direction;  // Direction pour les lumières directionnelles
    float m_range;     // Portée pour les lumières ponctuelles/spot
};

} // namespace ygl
