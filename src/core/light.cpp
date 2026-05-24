#include "core/light.hpp"

namespace ygl {

// Constructeur avec Light::Type (qualifié)
Light::Light(Light::Type type)
    : Object3D("Light"),
      m_type(type), m_color(1.0f, 1.0f, 1.0f),
      m_intensity(1.0f), m_direction(0.0f, -1.0f, 0.0f), m_range(10.0f) {}

Light::Type Light::getType() const { return m_type; }
void Light::setType(Light::Type type) { m_type = type; }

const Vec3& Light::getColor() const { return m_color; }
void Light::setColor(const Vec3& color) { m_color = color; }

float Light::getIntensity() const { return m_intensity; }
void Light::setIntensity(float intensity) { m_intensity = intensity; }

const Vec3& Light::getDirection() const { return m_direction; }
void Light::setDirection(const Vec3& direction) { m_direction = direction.normalize(); }

const Vec3& Light::getPosition() const { return Object3D::getPosition(); }
void Light::setPosition(const Vec3& position) { Object3D::setPosition(position); }

float Light::getRange() const { return m_range; }
void Light::setRange(float range) { m_range = range; }

} // namespace ygl
