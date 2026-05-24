#pragma once
#include <cmath>
#include <algorithm>
#include "math/vec3.hpp"

namespace ygl {
class Vec3;
class mat4;
class Quat {
public:
    union {
        struct { float x, y, z, w; };
    };
    Quat() : x(0), y(0), z(0), w(1) {}
    Quat(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
    Quat(const Vec3& axis, float angle) {
        float halfAngle = angle * 0.5f;
        float sinHalf = std::sin(halfAngle);
        x = axis.x * sinHalf;
        y = axis.y * sinHalf;
        z = axis.z * sinHalf;
        w = std::cos(halfAngle);
    }
    Quat Normalize() const {
        float len = std::sqrt(x * x + y * y + z * z + w * w);
        return len > 0 ? Quat(x / len, y / len, z / len, w / len) : Quat();
    }
    explicit Quat(const mat4& matrix);
    Quat operator+(const Quat& other) const { return Quat(x + other.x, y + other.y, z + other.z, w + other.w); }
    Quat operator-(const Quat& other) const { return Quat(x - other.x, y - other.y, z - other.z, w - other.w); }
    Quat operator*(float scalar) const { return Quat(x * scalar, y * scalar, z * scalar, w * scalar); }
    Quat operator*(const Quat& other) const {
        return Quat(
            w * other.x + x * other.w + y * other.z - z * other.y,
            w * other.y - x * other.z + y * other.w + z * other.x,
            w * other.z + x * other.y - y * other.x + z * other.w,
            w * other.w - x * other.x - y * other.y - z * other.z
        );
    }
    Vec3 operator*(const Vec3& vec) const;
    Quat operator-() const { return Quat(-x, -y, -z, -w); }
    static Quat identity() { return Quat(0.0f, 0.0f, 0.0f, 1.0f); }
    mat4 toMatrix() const;
};
} // namespace ygl
