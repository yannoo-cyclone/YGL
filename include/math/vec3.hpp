#pragma once

#include <cmath>
#include <algorithm>
#include <limits>
#include <ostream>

namespace ygl {

class Vec3 {
public:
    union {
        struct { float x, y, z; };
        struct { float r, g, b; };
    };

    Vec3() : x(0), y(0), z(0) {}
    explicit Vec3(float scalar) : x(scalar), y(scalar), z(scalar) {}
    Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

    Vec3 operator+(const Vec3& other) const { return Vec3(x + other.x, y + other.y, z + other.z); }
    Vec3 operator-(const Vec3& other) const { return Vec3(x - other.x, y - other.y, z - other.z); }
    Vec3 operator*(float scalar) const { return Vec3(x * scalar, y * scalar, z * scalar); }
    Vec3 operator*(const Vec3& other) const { return Vec3(x * other.x, y * other.y, z * other.z); }
    Vec3 operator-() const { return Vec3(-x, -y, -z); }
    Vec3 operator/(float scalar) const { return Vec3(x / scalar, y / scalar, z / scalar); }
    Vec3& operator+=(const Vec3& other) { x += other.x; y += other.y; z += other.z; return *this; }

    bool operator==(const Vec3& other) const { return x == other.x && y == other.y && z == other.z; }
    bool operator!=(const Vec3& other) const { return !(*this == other); }

    float Dot(const Vec3& other) const { return x * other.x + y * other.y + z * other.z; }
    float Length() const { return std::sqrt(x * x + y * y + z * z); }
    float lengthSquared() const { return x * x + y * y + z * z; }
    Vec3 Normalize() const { float len = Length(); return len > 0 ? *this / len : Vec3(); }

    static Vec3 Min(const Vec3& a, const Vec3& b) {
        return Vec3(std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z));
    }
    static Vec3 Max(const Vec3& a, const Vec3& b) {
        return Vec3(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z));
    }
    static Vec3 Clamp(const Vec3& v, const Vec3& min, const Vec3& max) {
        return Vec3(
            std::clamp(v.x, min.x, max.x),
            std::clamp(v.y, min.y, max.y),
            std::clamp(v.z, min.z, max.z)
        );
    }

    static Vec3 zero() { return Vec3(0.0f); }
    static Vec3 one() { return Vec3(1.0f); }
    static Vec3 unitX() { return Vec3(1.0f, 0.0f, 0.0f); }
    static Vec3 unitY() { return Vec3(0.0f, 1.0f, 0.0f); }
    static Vec3 unitZ() { return Vec3(0.0f, 0.0f, 1.0f); }

    Vec3 Transform(const class mat4& matrix) const;
};

inline std::ostream& operator<<(std::ostream& os, const Vec3& vec) {
    os << "Vec3(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
    return os;
}

inline Vec3 normalize(const Vec3& v) { return v.Normalize(); }
inline Vec3 cross(const Vec3& a, const Vec3& b) {
    return Vec3(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    );
}
inline float dot(const Vec3& a, const Vec3& b) { return a.Dot(b); }

} // namespace ygl
