#pragma once

#include "vec3.hpp"
#include "mat4.hpp"
#include <iostream>

namespace ygl {

class Quat {
public:
    union {
        struct { float x, y, z, w; };
        float data[4];
    };

    Quat();
    Quat(float x, float y, float z, float w);
    Quat(const Vec3& axis, float angle);
    explicit Quat(const Vec4& v);
    explicit Quat(const Mat4& m);

    float& operator[](int index) { return data[index]; }
    const float& operator[](int index) const { return data[index]; }

    Quat& operator+=(const Quat& other);
    Quat& operator-=(const Quat& other);
    Quat& operator*=(const Quat& other);
    Quat& operator*=(float scalar);
    Quat& operator/=(float scalar);

    Quat operator+(const Quat& other) const;
    Quat operator-(const Quat& other) const;
    Quat operator-() const;
    Quat operator*(const Quat& other) const;
    Quat operator*(float scalar) const;
    Quat operator/(float scalar) const;

    Vec3 operator*(const Vec3& v) const;

    bool operator==(const Quat& other) const;
    bool operator!=(const Quat& other) const;

    Quat normalized() const;
    void normalize();
    Quat conjugate() const;
    Quat inverse() const;
    float length() const;
    float lengthSquared() const;
    float dot(const Quat& other) const;
    Vec3 eulerAngles() const;
    Mat4 toMatrix() const;

    static Quat identity();
    static Quat fromEulerAngles(const Vec3& euler);
    static Quat lookAt(const Vec3& direction, const Vec3& up);
    static Quat slerp(const Quat& a, const Quat& b, float t);
    static Quat lerp(const Quat& a, const Quat& b, float t);

    friend std::ostream& operator<<(std::ostream& os, const Quat& q);
};

Quat operator*(float scalar, const Quat& q);
float dot(const Quat& a, const Quat& b);
Quat normalize(const Quat& q);
Quat conjugate(const Quat& q);
Quat inverse(const Quat& q);
Quat slerp(const Quat& a, const Quat& b, float t);
Quat lerp(const Quat& a, const Quat& b, float t);

} // namespace ygl

