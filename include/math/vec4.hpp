#pragma once

#include <cmath>
#include <algorithm>
#include <limits>
#include <iostream>

namespace ygl {

class Vec3;

class Vec4 {
public:
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wpedantic"
    union {
        struct { float x, y, z, w; };
        struct { float r, g, b, a; };
    };
    #pragma GCC diagnostic pop

    // Constructeurs
    Vec4();
    explicit Vec4(float scalar);
    Vec4(float x, float y, float z, float w);
    Vec4(const Vec3& v, float w);

    // Opérateurs de modification
    Vec4& operator+=(const Vec4& other);
    Vec4& operator-=(const Vec4& other);
    Vec4& operator*=(float scalar);
    Vec4& operator/=(float scalar);

    // Opérateurs arithmétiques
    Vec4 operator+(const Vec4& other) const;
    Vec4 operator-(const Vec4& other) const;
    Vec4 operator-() const;
    Vec4 operator*(float scalar) const;
    Vec4 operator/(float scalar) const;

    // Comparaison
    bool operator==(const Vec4& other) const;
    bool operator!=(const Vec4& other) const;

    // Calculs
    float length() const;
    float lengthSquared() const;
    Vec4 normalized() const;
    void normalize();
    float dot(const Vec4& other) const;

    // Méthodes statiques
    static Vec4 zero();
    static Vec4 one();

    // Output
    friend std::ostream& operator<<(std::ostream& os, const Vec4& v);
};

// Fonctions non-member
Vec4 operator*(float scalar, const Vec4& v);
float dot(const Vec4& a, const Vec4& b);
Vec4 normalize(const Vec4& v);

} // namespace ygl
