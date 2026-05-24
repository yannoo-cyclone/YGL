#include "core/light.hpp"

namespace ygl {

Light::Type Light::getType() const { return m_type; }
void Light::setType(Type type) { m_type = type; }

const Vec3& Light::getColor() const { return m_color; }
void Light::setColor(const Vec3& color) { m_color = color; }

float Light::getIntensity() const { return m_intensity; }
void Light::setIntensity(float intensity) { m_intensity = intensity; }

const Vec3& Light::getDirection() const { return m_direction; }
void Light::setDirection(const Vec3& direction) { m_direction = direction.Normalize(); }

const Vec3& Light::getPosition() const { return m_position; }
void Light::setPosition(const Vec3& position) { m_position = position; }

float Light::getRange() const { return m_range; }
void Light::setRange(float range) { m_range = range; }

} // namespace ygl
