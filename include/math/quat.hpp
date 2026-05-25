#pragma once

#include <cmath>
#include <algorithm>
#include "math/vec3.hpp"

namespace ygl {

class mat4;

class Quat {
public:
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wpedantic"
    union {
        struct { float x, y, z, w; };
    };
    #pragma GCC diagnostic pop

    // Constructeurs
    Quat();
    Quat(float x, float y, float z, float w);
    Quat(const Vec3& axis, float angle);

    // Opérateurs
    Quat operator+(const Quat& other) const;
    Quat operator-(const Quat& other) const;
    Quat operator*(float scalar) const;
    Quat operator*(const Quat& other) const;
    Quat operator-() const;

    // Transformation
    Vec3 operator*(const Vec3& vec) const;

    // Normalisation
    Quat Normalize() const;
    static Quat identity();

    // Conversion
    explicit Quat(const mat4& matrix);
    mat4 toMatrix() const;
};

} // namespace ygl
