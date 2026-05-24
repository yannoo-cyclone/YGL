#include "math/vec3.hpp"
#include "math/vec2.hpp"
#include "math/vec4.hpp"
#include <cmath>
#include <algorithm>

namespace ygl {

// Constructors
Vec3::Vec3(const Vec2& v, float z) : x(v.x), y(v.y), z(z) {}
Vec3::Vec3(const Vec4& v) : x(v.x), y(v.y), z(v.z) {}

// Assignment
Vec3& Vec3::operator+=(const Vec3& other) {
    x += other.x; y += other.y; z += other.z; return *this;
}
Vec3& Vec3::operator-=(const Vec3& other) {
    x -= other.x; y -= other.y; z -= other.z; return *this;
}
Vec3& Vec3::operator*=(float scalar) {
    x *= scalar; y *= scalar; z *= scalar; return *this;
}
Vec3& Vec3::operator/=(float scalar) {
    float inv = 1.0f / scalar; x *= inv; y *= inv; z *= inv; return *this;
}

// Arithmetic
Vec3 Vec3::operator+(const Vec3& other) const {
    return Vec3(x+other.x, y+other.y, z+other.z);
}
Vec3 Vec3::operator-(const Vec3& other) const {
    return Vec3(x-other.x, y-other.y, z-other.z);
}
Vec3 Vec3::operator-() const { return Vec3(-x, -y, -z); }
Vec3 Vec3::operator*(float scalar) const {
    return Vec3(x*scalar, y*scalar, z*scalar);
}
Vec3 Vec3::operator/(float scalar) const {
    float inv = 1.0f / scalar; return Vec3(x*inv, y*inv, z*inv);
}

// Comparison
bool Vec3::operator==(const Vec3& other) const {
    return x == other.x && y == other.y && z == other.z;
}
bool Vec3::operator!=(const Vec3& other) const {
    return !(*this == other);
}

// Vector operations
float Vec3::length() const { return std::sqrt(x*x + y*y + z*z); }
float Vec3::lengthSquared() const { return x*x + y*y + z*z; }
Vec3 Vec3::normalized() const {
    float len = length(); return len > 0.0f ? *this / len : *this;
}
void Vec3::normalize() { *this = normalized(); }
float Vec3::dot(const Vec3& other) const {
    return x*other.x + y*other.y + z*other.z;
}
Vec3 Vec3::cross(const Vec3& other) const {
    return Vec3(
        y*other.z - z*other.y,
        z*other.x - x*other.z,
        x*other.y - y*other.x
    );
}
float Vec3::distanceTo(const Vec3& other) const {
    return std::sqrt((x-other.x)*(x-other.x) + (y-other.y)*(y-other.y) + (z-other.z)*(z-other.z));
}
Vec3 Vec3::lerp(const Vec3& other, float t) const {
    return Vec3(x+(other.x-x)*t, y+(other.y-y)*t, z+(other.z-z)*t);
}
Vec3 Vec3::reflect(const Vec3& normal) const {
    return *this - normal * 2.0f * dot(normal);
}
Vec3 Vec3::refract(const Vec3& normal, float ior) const {
    float dotValue = dot(normal);
    float k = 1.0f - ior*ior * (1.0f - dotValue*dotValue);
    if (k < 0.0f) return Vec3(0.0f);
    return *this * ior - normal * (ior * dotValue + std::sqrt(k));
}

// Utility
bool Vec3::isZero() const { return x == 0.0f && y == 0.0f && z == 0.0f; }
bool Vec3::isNormalized() const {
    return std::abs(lengthSquared() - 1.0f) < 0.0001f;
}
Vec3 Vec3::abs() const { return Vec3(std::abs(x), std::abs(y), std::abs(z)); }
Vec3 Vec3::clamp(float min, float max) const {
    return Vec3(std::clamp(x, min, max), std::clamp(y, min, max), std::clamp(z, min, max));
}
Vec3 Vec3::clamp(const Vec3& min, const Vec3& max) const {
    return Vec3(std::clamp(x, min.x, max.x), std::clamp(y, min.y, max.y), std::clamp(z, min.z, max.z));
}

// Output
std::ostream& operator<<(std::ostream& os, const Vec3& v) {
    os << "Vec3(" << v.x << ", " << v.y << ", " << v.z << ")"; return os;
}

// Non-member
Vec3 operator*(float scalar, const Vec3& v) { return v * scalar; }
float dot(const Vec3& a, const Vec3& b) { return a.dot(b); }
Vec3 cross(const Vec3& a, const Vec3& b) { return a.cross(b); }
Vec3 normalize(const Vec3& v) { return v.normalized(); }
float length(const Vec3& v) { return v.length(); }
float lengthSquared(const Vec3& v) { return v.lengthSquared(); }
Vec3 lerp(const Vec3& a, const Vec3& b, float t) { return a.lerp(b, t); }
Vec3 reflect(const Vec3& v, const Vec3& normal) { return v.reflect(normal); }
Vec3 refract(const Vec3& v, const Vec3& normal, float ior) { return v.refract(normal, ior); }

} // namespace ygl
