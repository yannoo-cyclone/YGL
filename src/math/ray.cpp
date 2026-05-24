#include "math/ray.hpp"
#include "math/math_utils.hpp"

namespace ygl {

// --- Ray Constructors ---
Ray::Ray()
    : origin(0.0f),
      direction(0.0f, 0.0f, 1.0f),
      t_min(0.0f),
      t_max(std::numeric_limits<float>::max()) {
}

Ray::Ray(const vec3& origin, const vec3& direction, float t_min, float t_max)
    : origin(origin),
      direction(normalize(direction)),
      t_min(t_min),
      t_max(t_max) {
}

// --- Access ---
vec3 Ray::operator()(float t) const {
    return origin + direction * t;
}

vec3 Ray::at(float t) const {
    return (*this)(t);
}

// --- Getters/Setters ---
const vec3& Ray::GetOrigin() const {
    return origin;
}

const vec3& Ray::GetDirection() const {
    return direction;
}

float Ray::GetTMin() const {
    return t_min;
}

float Ray::GetTMax() const {
    return t_max;
}

void Ray::SetOrigin(const vec3& o) {
    origin = o;
}

void Ray::SetDirection(const vec3& d) {
    direction = normalize(d);
}

void Ray::SetTMin(float t) {
    t_min = t;
}

void Ray::SetTMax(float t) {
    t_max = t;
}

void Ray::SetBounds(float min, float max) {
    t_min = min;
    t_max = max;
}

// --- Check validity ---
bool Ray::is_valid() const {
    return !std::isnan(direction.x) &&
           !std::isnan(direction.y) &&
           !std::isnan(direction.z) &&
           direction.length_squared() > 0.0f;
}

// --- Transform ray by matrix ---
Ray Ray::transform(const mat4& matrix) const {
    vec4 origin_homogeneous = matrix * vec4(origin, 1.0f);
    vec4 direction_homogeneous = matrix * vec4(direction, 0.0f);

    vec3 new_origin = vec3(origin_homogeneous) / origin_homogeneous.w;
    vec3 new_direction = vec3(direction_homogeneous);

    // Avoid division by zero for direction
    if (new_direction.length_squared() < EPSILON) {
        new_direction = vec3(0.0f, 0.0f, 1.0f);
    } else {
        new_direction = normalize(new_direction);
    }

    return Ray(new_origin, new_direction, t_min, t_max);
}

// --- DifferentialRay Constructors ---
DifferentialRay::DifferentialRay()
    : Ray(),
      has_differentials(false) {
}

DifferentialRay::DifferentialRay(const vec3& origin, const vec3& direction, float t_min, float t_max)
    : Ray(origin, direction, t_min, t_max),
      has_differentials(false) {
}

DifferentialRay::DifferentialRay(
    const vec3& origin, const vec3& direction,
    const vec3& rx_origin, const vec3& ry_origin,
    const vec3& rx_direction, const vec3& ry_direction,
    float t_min, float t_max
) : Ray(origin, direction, t_min, t_max),
    rx_origin(rx_origin),
    ry_origin(ry_origin),
    rx_direction(rx_direction),
    ry_direction(ry_direction),
    has_differentials(true) {
}

// --- Differential access ---
vec3 DifferentialRay::get_rx_origin() const {
    return rx_origin;
}

vec3 DifferentialRay::get_ry_origin() const {
    return ry_origin;
}

vec3 DifferentialRay::get_rx_direction() const {
    return rx_direction;
}

vec3 DifferentialRay::get_ry_direction() const {
    return ry_direction;
}

bool DifferentialRay::get_has_differentials() const {
    return has_differentials;
}

void DifferentialRay::set_has_differentials(bool has) {
    has_differentials = has;
}

void DifferentialRay::set_rx_origin(const vec3& o) {
    rx_origin = o;
    has_differentials = true;
}

void DifferentialRay::set_ry_origin(const vec3& o) {
    ry_origin = o;
    has_differentials = true;
}

void DifferentialRay::set_rx_direction(const vec3& d) {
    rx_direction = d;
    has_differentials = true;
}

void DifferentialRay::set_ry_direction(const vec3& d) {
    ry_direction = d;
    has_differentials = true;
}

// --- Scale differentials ---
void DifferentialRay::scale_differentials(float s) {
    if (has_differentials) {
        rx_origin = origin + (rx_origin - origin) * s;
        ry_origin = origin + (ry_origin - origin) * s;
        rx_direction = direction + (rx_direction - direction) * s;
        ry_direction = direction + (ry_direction - direction) * s;
    }
}

// --- Transform differential ray by matrix ---
DifferentialRay DifferentialRay::transform(const mat4& matrix) const {
    DifferentialRay result;

    // Transform main ray
    vec4 origin_homogeneous = matrix * vec4(origin, 1.0f);
    vec4 direction_homogeneous = matrix * vec4(direction, 0.0f);

    result.origin = vec3(origin_homogeneous) / origin_homogeneous.w;
    result.direction = normalize(vec3(direction_homogeneous));
    result.t_min = t_min;
    result.t_max = t_max;

    // Transform differentials if present
    if (has_differentials) {
        vec4 rx_origin_homogeneous = matrix * vec4(rx_origin, 1.0f);
        vec4 ry_origin_homogeneous = matrix * vec4(ry_origin, 1.0f);
        vec4 rx_direction_homogeneous = matrix * vec4(rx_direction, 0.0f);
        vec4 ry_direction_homogeneous = matrix * vec4(ry_direction, 0.0f);

        result.rx_origin = vec3(rx_origin_homogeneous) / rx_origin_homogeneous.w;
        result.ry_origin = vec3(ry_origin_homogeneous) / ry_origin_homogeneous.w;
        result.rx_direction = normalize(vec3(rx_direction_homogeneous));
        result.ry_direction = normalize(vec3(ry_direction_homogeneous));
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
