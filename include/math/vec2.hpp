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
        float data[2];
    };

    Vec2() : x(0.0f), y(0.0f) {}
    Vec2(float scalar) : x(scalar), y(scalar) {}
    Vec2(float x, float y) : x(x), y(y) {}
    Vec2(const float* array) : x(array[0]), y(array[1]) {}

    float& operator[](int index) { return data[index]; }
    const float& operator[](int index) const { return data[index]; }

    Vec2& operator+=(const Vec2& other) { x += other.x; y += other.y; return *this; }
    Vec2& operator-=(const Vec2& other) { x -= other.x; y -= other.y; return *this; }
    Vec2& operator*=(float scalar) { x *= scalar; y *= scalar; return *this; }
    Vec2& operator/=(float scalar) { x /= scalar; y /= scalar; return *this; }

    Vec2 operator+(const Vec2& other) const { return Vec2(x + other.x, y + other.y); }
    Vec2 operator-(const Vec2& other) const { return Vec2(x - other.x, y - other.y); }
    Vec2 operator-() const { return Vec2(-x, -y); }
    Vec2 operator*(float scalar) const { return Vec2(x * scalar, y * scalar); }
    Vec2 operator/(float scalar) const { return Vec2(x / scalar, y / scalar); }

    bool operator==(const Vec2& other) const { return x == other.x && y == other.y; }
    bool operator!=(const Vec2& other) const { return !(*this == other); }

    float length() const { return std::sqrt(x * x + y * y); }
    float lengthSquared() const { return x * x + y * y; }
    Vec2 normalized() const { float len = length(); return len > 0 ? *this / len : Vec2(); }
    void normalize() { *this = normalized(); }
    float dot(const Vec2& other) const { return x * other.x + y * other.y; }
    float distanceTo(const Vec2& other) const { return (*this - other).length(); }
    Vec2 lerp(const Vec2& other, float t) const { return *this + (other - *this) * t; }

    static Vec2 zero() { return Vec2(0.0f); }
    static Vec2 one() { return Vec2(1.0f); }
    static Vec2 unitX() { return Vec2(1.0f, 0.0f); }
    static Vec2 unitY() { return Vec2(0.0f, 1.0f); }

    friend std::ostream& operator<<(std::ostream& os, const Vec2& v) {
        os << "Vec2(" << v.x << ", " << v.y << ")";
        return os;
    }
};

Vec2 operator*(float scalar, const Vec2& v) { return v * scalar; }
float dot(const Vec2& a, const Vec2& b) { return a.dot(b); }
Vec2 normalize(const Vec2& v) { return v.normalized(); }

} // namespace ygl
