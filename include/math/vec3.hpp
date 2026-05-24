#pragma once

#include <cmath>
#include <algorithm>  // Pour std::min/max
#include <limits>

namespace ygl {

class Vec3 {
public:
    union {
        struct { float x, y, z; };
        struct { float r, g, b; };
    };

    // Constructeurs
    Vec3() : x(0), y(0), z(0) {}
    Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

    // Opérateurs
    Vec3 operator+(const Vec3& other) const { return Vec3(x + other.x, y + other.y, z + other.z); }
    Vec3 operator-(const Vec3& other) const { return Vec3(x - other.x, y - other.y, z - other.z); }
    Vec3 operator*(float scalar) const { return Vec3(x * scalar, y * scalar, z * scalar); }
    Vec3 operator-() const { return Vec3(-x, -y, -z); }

    // Produit scalaire
    float Dot(const Vec3& other) const { return x * other.x + y * other.y + z * other.z; }

    // Norme
    float Length() const { return std::sqrt(x * x + y * y + z * z); }

    // Normalisation
    Vec3 Normalize() const { float len = Length(); return len > 0 ? *this / len : Vec3(); }

    // Opérateur de division
    Vec3 operator/(float scalar) const { return Vec3(x / scalar, y / scalar, z / scalar); }

    // Méthodes statiques pour Min/Max/Clamp
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

    // Transformation par une matrice (à ajouter si mat4.hpp est inclus)
    Vec3 Transform(const class mat4& matrix) const;
};

} // namespace ygl
