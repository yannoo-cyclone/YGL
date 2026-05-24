#pragma once

#include <cmath>
#include <iostream>
#include <array>

namespace ygl {

class Vec2;
class Vec4;

class Vec3 {
public:
    union {
        struct { float x, y, z; };
        struct { float r, g, b; };
        float data[3];
    };

    Vec3() : x(0.0f), y(0.0f), z(0.0f) {}
    Vec3(float scalar) : x(scalar), y(scalar), z(scalar) {}
    Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
    Vec3(const float* array) : x(array[0]), y(array[1]), z(array[2]) {}
    explicit Vec3(const Vec2& v, float z = 0.0f);
    explicit Vec3(const Vec4& v);

    float& operator[](int index) { return data[index]; }
    const float& operator[](int index) const { return data[index]; }

    Vec3& operator+=(const Vec3& other);
    Vec3& operator-=(const Vec3& other);
    Vec3& operator*=(float scalar);
    Vec3& operator/=(float scalar);

    Vec3 operator+(const Vec3& other) const;
    Vec3 operator-(const Vec3& other) const;
    Vec3 operator-() const;
    Vec3 operator*(float scalar) const;
    Vec3 operator/(float scalar) const;

    bool operator==(const Vec3& other) const;
    bool operator!=(const Vec3& other) const;

    float length() const;
    float lengthSquared() const;
    Vec3 normalized() const;
    void normalize();
    float dot(const Vec3& other) const;
    Vec3 cross(const Vec3& other) const;
    float distanceTo(const Vec3& other) const;
    Vec3 lerp(const Vec3& other, float t) const;
    Vec3 reflect(const Vec3& normal) const;
    Vec3 refract(const Vec3& normal, float ior) const;

    bool isZero() const;
    bool isNormalized() const;
    Vec3 abs() const;
    Vec3 clamp(float min, float max) const;
    Vec3 clamp(const Vec3& min, const Vec3& max) const;

    static Vec3 zero() { return Vec3(0.0f); }
    static Vec3 one() { return Vec3(1.0f); }
    static Vec3 unitX() { return Vec3(1.0f, 0.0f, 0.0f); }
    static Vec3 unitY() { return Vec3(0.0f, 1.0f, 0.0f); }
    static Vec3 unitZ() { return Vec3(0.0f, 0.0f, 1.0f); }

    friend std::ostream& operator<<(std::ostream& os, const Vec3& v);
};

Vec3 operator*(float scalar, const Vec3& v);
float dot(const Vec3& a, const Vec3& b);
Vec3 cross(const Vec3& a, const Vec3& b);
Vec3 normalize(const Vec3& v);
float length(const Vec3& v);
float lengthSquared(const Vec3& v);
Vec3 lerp(const Vec3& a, const Vec3& b, float t);
Vec3 reflect(const Vec3& v, const Vec3& normal);
Vec3 refract(const Vec3& v, const Vec3& normal, float ior);

} // namespace ygl

