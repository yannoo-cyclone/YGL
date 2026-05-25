#pragma once
#include <cmath>
#include <algorithm>

namespace ygl {
class Vec3; class Mat4;
class Quat {
public:
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wpedantic"
    union { struct { float x, y, z, w; }; };
    #pragma GCC diagnostic pop
    Quat(); Quat(float x, float y, float z, float w); Quat(const Vec3& axis, float angle);
    Quat operator+(const Quat& other) const; Quat operator-(const Quat& other) const;
    Quat operator*(float scalar) const; Quat operator*(const Quat& other) const; Quat operator-() const;
    Vec3 operator*(const Vec3& vec) const;
    Quat Normalize() const; static Quat identity();
    explicit Quat(const Mat4& matrix); Mat4 toMatrix() const;
};
} // namespace ygl
