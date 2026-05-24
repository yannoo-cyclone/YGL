#pragma once

#include <cmath>
#include <iostream>

namespace ygl {

class Vec3;

class Vec2 {
public:
    union {
        struct { float x, y; };
        struct { float u, v; };
        float data[2];  // AJOUTÉ pour operator[]
    };

    // Constructeurs
    Vec2() : x(0.0f), y(0.0f) {}
    Vec2(float scalar) : x(scalar), y(scalar) {}
    Vec2(float x, float y) : x(x), y(y) {}
    Vec2(const float* array) : x(array[0]), y(array[1]) {}

    // Accès
    float& operator[](int index) { return data[index]; }
    const float& operator[](int index) const { return data[index]; }

    // Opérateurs d'assignation
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

    // Opérations vectorielles
    float length() const;
    float lengthSquared() const;
    Vec2 normalized() const;
    void normalize();
    float dot(const Vec2& other) const;
    float distanceTo(const Vec2& other) const;
    Vec2 lerp(const Vec2& other, float t) const;

    // Constantes
    static Vec2 zero() { return Vec2(0.0f); }
    static Vec2 one() { return Vec2(1.0f); }
    static Vec2 unitX() { return Vec2(1.0f, 0.0f); }
    static Vec2 unitY() { return Vec2(0.0f, 1.0f); }

    // Sortie
    friend std::ostream& operator<<(std::ostream& os, const Vec2& v);
};

// Fonctions non-membres
Vec2 operator*(float scalar, const Vec2& v);
float dot(const Vec2& a, const Vec2& b);
Vec2 normalize(const Vec2& v);

} // namespace ygl
