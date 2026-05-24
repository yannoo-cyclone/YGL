#pragma once

#include <cmath>
#include <algorithm>
#include <limits>

namespace ygl {

class Vec4 {
public:
    union {
        struct { float x, y, z, w; };
        struct { float r, g, b, a; };
    };

    // Constructeurs
    Vec4() : x(0), y(0), z(0), w(0) {}
    Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

    // Opérateurs
    Vec4 operator+(const Vec4& other) const { return Vec4(x + other.x, y + other.y, z + other.z, w + other.w); }
    Vec4 operator-(const Vec4& other) const { return Vec4(x - other.x, y - other.y, z - other.z, w - other.w); }
    Vec4 operator*(float scalar) const { return Vec4(x * scalar, y * scalar, z * scalar, w * scalar); }
    Vec4 operator/(float scalar) const { return Vec4(x / scalar, y / scalar, z / scalar, w / scalar); }
    Vec4 operator-() const { return Vec4(-x, -y, -z, -w); }
    Vec4 operator*(const Vec4& other) const { return Vec4(x * other.x, y * other.y, z * other.z, w * other.w); }

    // Norme
    float Length() const { return std::sqrt(x * x + y * y + z * z + w * w); }

    // Méthodes statiques
    static Vec4 Min(const Vec4& a, const Vec4& b) {
        return Vec4(std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z), std::min(a.w, b.w));
    }
    static Vec4 Max(const Vec4& a, const Vec4& b) {
        return Vec4(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z), std::max(a.w, b.w));
    }
    static Vec4 Clamp(const Vec4& v, const Vec4& min, const Vec4& max) {
        return Vec4(
            std::clamp(v.x, min.x, max.x),
            std::clamp(v.y, min.y, max.y),
            std::clamp(v.z, min.z, max.z),
            std::clamp(v.w, min.w, max.w)
        );
    }
};

} // namespace ygl
