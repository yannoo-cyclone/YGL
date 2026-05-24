#pragma once

#include <cmath>
#include <algorithm>

namespace ygl {

class Vec3;
class mat4;

class Quat {
public:
    union {
        struct { float x, y, z, w; };
    };

    // Constructeurs
    Quat() : x(0), y(0), z(0), w(1) {}
    Quat(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

    // Normalisation
    Quat Normalize() const {
        float len = std::sqrt(x * x + y * y + z * z + w * w);
        return len > 0 ? Quat(x / len, y / len, z / len, w / len) : Quat();
    }

    // Constructeur depuis une matrice 4x4
    explicit Quat(const mat4& matrix);

    // Opérateurs
    Quat operator+(const Quat& other) const { return Quat(x + other.x, y + other.y, z + other.z, w + other.w); }
    Quat operator-(const Quat& other) const { return Quat(x - other.x, y - other.y, z - other.z, w - other.w); }
    Quat operator*(float scalar) const { return Quat(x * scalar, y * scalar, z * scalar, w * scalar); }
    Quat operator-() const { return Quat(-x, -y, -z, -w); }
};

} // namespace ygl
