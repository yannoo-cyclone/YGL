#pragma once

#include <cmath>
#include <iostream>

namespace ygl {

class Vec2 {
public:
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wpedantic"
    union {
        struct { float x, y; };
        struct { float u, v; };
        float data[2];
    };
    #pragma GCC diagnostic pop

    // Constructeurs
    Vec2();
    explicit Vec2(float scalar);
    Vec2(float x, float y);
    Vec2(const float* array);

    // Accès
    float& operator[](int index);
    const float& operator[](int index) const;

    // Opérateurs de modification
    Vec2& operator+=(const Vec2& other);
    Vec2& operator-=(const Vec2& other);
    Vec2& operator*=(float scalar);
    Vec2& operator/=(float scalar);

    // Opérateurs arithmétiques
    Vec2 operator+(const Vec2& other) const;
    Vec2 operator-(const Vec2& other) const;
    Vec2 operator-() const;
    Vec2 operator*(float scalar) const;
    Vec2 operator/(float scalar) const;

    // Comparaison
    bool operator==(const Vec2& other) const;
    bool operator!=(const Vec2& other) const;

    // Calculs
    float length() const;
    float lengthSquared() const;
    Vec2 normalized() const;
    void normalize();
    float dot(const Vec2& other) const;

    // Méthodes statiques
    static Vec2 zero();
    static Vec2 one();
    static Vec2 unitX();
    static Vec2 unitY();

    // Output
    friend std::ostream& operator<<(std::ostream& os, const Vec2& v);
};

// Opérateur non-member
Vec2 operator*(float scalar, const Vec2& v);

} // namespace ygl
