#pragma once

#include <cmath>
#include <algorithm>
#include <limits>
#include <iostream>

namespace ygl {

class mat4;

class Vec3 {
public:
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wpedantic"
    union {
        struct { float x, y, z; };
        struct { float r, g, b; };
    };
    #pragma GCC diagnostic pop

    // Constructeurs
    Vec3();
    explicit Vec3(float scalar);
    Vec3(float x, float y, float z);

    // Opérateurs de modification
    Vec3& operator+=(const Vec3& other);
    Vec3& operator-=(const Vec3& other);
    Vec3& operator*=(float scalar);
    Vec3& operator/=(float scalar);

    // Opérateurs arithmétiques
    Vec3 operator+(const Vec3& other) const;
    Vec3 operator-(const Vec3& other) const;
    Vec3 operator-() const;
    Vec3 operator*(float scalar) const;
    Vec3 operator/(float scalar) const;

    // Comparaison
    bool operator==(const Vec3& other) const;
    bool operator!=(const Vec3& other) const;

    // Calculs
    float dot(const Vec3& other) const;
    Vec3 cross(const Vec3& other) const;
    float length() const;
    float lengthSquared() const;
    Vec3 normalized() const;
    void normalize();

    // Transformation
    Vec3 Transform(const mat4& matrix) const;

    // Méthodes statiques
    static Vec3 zero();
    static Vec3 one();
    static Vec3 unitX();
    static Vec3 unitY();
    static Vec3 unitZ();

    // Output
    friend std::ostream& operator<<(std::ostream& os, const Vec3& vec);
};

// Fonctions non-member
Vec3 operator*(float scalar, const Vec3& v);
Vec3 normalize(const Vec3& v);
float dot(const Vec3& a, const Vec3& b);
Vec3 cross(const Vec3& a, const Vec3& b);

} // namespace ygl
