#pragma once

#include <cmath>
#include <iostream>

namespace ygl {

class Vec3;

class Vec4 {
public:
    union {
        struct { float x, y, z, w; };
        struct { float r, g, b, a; };
        float data[4];
    };

    Vec4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
    Vec4(float scalar) : x(scalar), y(scalar), z(scalar), w(scalar) {}
    Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
    Vec4(const float* array) : x(array[0]), y(array[1]), z(array[2]), w(array[3]) {}
    explicit Vec4(const Vec3& v, float w = 1.0f);

    float& operator[](int index) { return data[index]; }
    const float& operator[](int index) const { return data[index]; }

    Vec4& operator+=(const Vec4& other);
    Vec4& operator-=(const Vec4& other);
    Vec4& operator*=(float scalar);
    Vec4& operator/=(float scalar);

    Vec4 operator+(const Vec4& other) const;
    Vec4 operator-(const Vec4& other) const;
    Vec4 operator-() const;
    Vec4 operator*(float scalar) const;
    Vec4 operator/(float scalar) const;

    bool operator==(const Vec4& other) const;
    bool operator!=(const Vec4& other) const;

    float length() const;
    float lengthSquared() const;
    Vec4 normalized() const;
    void normalize();
    float dot(const Vec4& other) const;

    static Vec4 zero() { return Vec4(0.0f); }
    static Vec4 one() { return Vec4(1.0f); }
    static Vec4 unitX() { return Vec4(1.0f, 0.0f, 0.0f, 0.0f); }
    static Vec4 unitY() { return Vec4(0.0f, 1.0f, 0.0f, 0.0f); }
    static Vec4 unitZ() { return Vec4(0.0f, 0.0f, 1.0f, 0.0f); }
    static Vec4 unitW() { return Vec4(0.0f, 0.0f, 0.0f, 1.0f); }

    friend std::ostream& operator<<(std::ostream& os, const Vec4& v);
};

Vec4 operator*(float scalar, const Vec4& v);
float dot(const Vec4& a, const Vec4& b);
Vec4 normalize(const Vec4& v);

} // namespace ygl

