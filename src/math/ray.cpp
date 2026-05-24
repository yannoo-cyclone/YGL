#include "math/ray.hpp"
#include "math/math_utils.hpp"
#include "math/mat4.hpp"

namespace ygl {

// --- Ray Constructors ---
Ray::Ray()
    : origin(0.0f, 0.0f, 0.0f),
      direction(0.0f, 0.0f, 1.0f),
      t_min(0.0f),
      t_max(std::numeric_limits<float>::max()) {
}

Ray::Ray(const Vec3& origin, const Vec3& direction)
    : origin(origin),
      direction(direction),
      t_min(0.0f),
      t_max(std::numeric_limits<float>::max()) {
}

// --- Access ---
Vec3 Ray::operator()(float t) const {
    return origin + direction * t;
}

Vec3 Ray::at(float t) const {
    return (*this)(t);
}

// --- Getters ---
const Vec3& Ray::getOrigin() const { return origin; }
const Vec3& Ray::getDirection() const { return direction; }
float Ray::getTMin() const { return t_min; }
float Ray::getTMax() const { return t_max; }

// --- Setters ---
void Ray::setOrigin(const Vec3& o) { origin = o; }
void Ray::setDirection(const Vec3& d) { direction = d; }
void Ray::setTMin(float t) { t_min = t; }
void Ray::setTMax(float t) { t_max = t; }
void Ray::setBounds(float min, float max) { t_min = min; t_max = max; }

// --- Validity ---
bool Ray::isValid() const {
    return !std::isnan(direction.x) &&
           !std::isnan(direction.y) &&
           !std::isnan(direction.z) &&
           direction.lengthSquared() > 0.0f;
}

// --- Transform ---
Ray Ray::transform(const mat4& matrix) const {
    Vec3 new_origin = matrix * Vec3(origin, 1.0f);
    Vec3 new_direction = matrix * Vec3(direction, 0.0f);
    return Ray(new_origin, new_direction);
}

// --- DifferentialRay Constructors ---
DifferentialRay::DifferentialRay()
    : Ray(), has_differentials(false) {}

DifferentialRay::DifferentialRay(const Vec3& origin, const Vec3& direction, float t_min, float t_max)
    : Ray(origin, direction), has_differentials(false) {
    this->t_min = t_min;
    this->t_max = t_max;
}

DifferentialRay::DifferentialRay(
    const Vec3& origin, const Vec3& direction,
    const Vec3& rx_origin, const Vec3& ry_origin,
    const Vec3& rx_direction, const Vec3& ry_direction,
    float t_min, float t_max
) : Ray(origin, direction),
    rx_origin(rx_origin), ry_origin(ry_origin),
    rx_direction(rx_direction), ry_direction(ry_direction),
    has_differentials(true) {
    this->t_min = t_min;
    this->t_max = t_max;
}

// --- DifferentialRay Getters ---
Vec3 DifferentialRay::getRxOrigin() const { return rx_origin; }
Vec3 DifferentialRay::getRyOrigin() const { return ry_origin; }
Vec3 DifferentialRay::getRxDirection() const { return rx_direction; }
Vec3 DifferentialRay::getRyDirection() const { return ry_direction; }
bool DifferentialRay::getHasDifferentials() const { return has_differentials; }

// --- DifferentialRay Setters ---
void DifferentialRay::setHasDifferentials(bool has) { has_differentials = has; }
void DifferentialRay::setRxOrigin(const Vec3& o) { rx_origin = o; has_differentials = true; }
void DifferentialRay::setRyOrigin(const Vec3& o) { ry_origin = o; has_differentials = true; }
void DifferentialRay::setRxDirection(const Vec3& d) { rx_direction = d; has_differentials = true; }
void DifferentialRay::setRyDirection(const Vec3& d) { ry_direction = d; has_differentials = true; }

// --- Scale differentials ---
void DifferentialRay::scaleDifferentials(float s) {
    if (has_differentials) {
        rx_origin = origin + (rx_origin - origin) * s;
        ry_origin = origin + (ry_origin - origin) * s;
        rx_direction = direction + (rx_direction - direction) * s;
        ry_direction = direction + (ry_direction - direction) * s;
    }
}

// --- Transform ---
DifferentialRay DifferentialRay::transform(const mat4& matrix) const {
    DifferentialRay result;
    result.origin = matrix * Vec3(origin, 1.0f);
    result.direction = (matrix * Vec3(direction, 0.0f)).normalized();
    result.t_min = t_min;
    result.t_max = t_max;
    if (has_differentials) {
        result.rx_origin = matrix * Vec3(rx_origin, 1.0f);
        result.ry_origin = matrix * Vec3(ry_origin, 1.0f);
        result.rx_direction = (matrix * Vec3(rx_direction, 0.0f)).normalized();
        result.ry_direction = (matrix * Vec3(ry_direction, 0.0f)).normalized();
        result.has_differentials = true;
    }
    return result;
}

// --- Output ---
std::ostream& operator<<(std::ostream& os, const Ray& ray) {
    os << "Ray(origin=" << ray.origin
       << ", direction=" << ray.direction
       << ", t_min=" << ray.t_min
       << ", t_max=" << ray.t_max << ")";
    return os;
}

std::ostream& operator<<(std::ostream& os, const DifferentialRay& ray) {
    os << "DifferentialRay(" << static_cast<const Ray&>(ray);
    if (ray.has_differentials) {
        os << ", rx_origin=" << ray.rx_origin
           << ", ry_origin=" << ray.ry_origin
           << ", rx_direction=" << ray.rx_direction
           << ", ry_direction=" << ray.ry_direction;
    }
    os << ")";
    return os;
}

} // namespace ygl
