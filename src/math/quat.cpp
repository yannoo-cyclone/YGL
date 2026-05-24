#include "math/quat.hpp"
#include "math/vec3.hpp"
#include "math/mat4.hpp"
#include <cmath>

namespace ygl {

// Constructeur depuis mat4 (SEUL constructeur non inline dans le .hpp)
Quat::Quat(const mat4& matrix) {
    float trace = matrix(0,0) + matrix(1,1) + matrix(2,2) + matrix(3,3);
    if (trace > 0) {
        float s = 0.5f / std::sqrt(trace + 1.0f);
        w = 0.25f / s;
        x = (matrix(2,1) - matrix(1,2)) * s;
        y = (matrix(0,2) - matrix(2,0)) * s;
        z = (matrix(1,0) - matrix(0,1)) * s;
    } else {
        if (matrix(0,0) > matrix(1,1) && matrix(0,0) > matrix(2,2)) {
            float s = 2.0f * std::sqrt(1.0f + matrix(0,0) - matrix(1,1) - matrix(2,2));
            w = (matrix(2,1) - matrix(1,2)) / s;
            x = 0.25f * s;
            y = (matrix(0,1) + matrix(1,0)) / s;
            z = (matrix(0,2) + matrix(2,0)) / s;
        } else if (matrix(1,1) > matrix(2,2)) {
            float s = 2.0f * std::sqrt(1.0f + matrix(1,1) - matrix(0,0) - matrix(2,2));
            w = (matrix(0,2) - matrix(2,0)) / s;
            x = (matrix(0,1) + matrix(1,0)) / s;
            y = 0.25f * s;
            z = (matrix(1,2) + matrix(2,1)) / s;
        } else {
            float s = 2.0f * std::sqrt(1.0f + matrix(2,2) - matrix(0,0) - matrix(1,1));
            w = (matrix(1,0) - matrix(0,1)) / s;
            x = (matrix(0,2) + matrix(2,0)) / s;
            y = (matrix(1,2) + matrix(2,1)) / s;
            z = 0.25f * s;
        }
    }
}

// toMatrix (non inline dans le .hpp)
mat4 Quat::toMatrix() const {
    mat4 m;
    float xx = x * x, yy = y * y, zz = z * z;
    float xy = x * y, xz = x * z, yz = y * z;
    float wx = w * x, wy = w * y, wz = w * z;

    m(0,0) = 1.0f - 2.0f * (yy + zz);
    m(0,1) = 2.0f * (xy - wz);
    m(0,2) = 2.0f * (xz + wy);
    m(0,3) = 0.0f;

    m(1,0) = 2.0f * (xy + wz);
    m(1,1) = 1.0f - 2.0f * (xx + zz);
    m(1,2) = 2.0f * (yz - wx);
    m(1,3) = 0.0f;

    m(2,0) = 2.0f * (xz - wy);
    m(2,1) = 2.0f * (yz + wx);
    m(2,2) = 1.0f - 2.0f * (xx + yy);
    m(2,3) = 0.0f;

    m(3,0) = 0.0f;
    m(3,1) = 0.0f;
    m(3,2) = 0.0f;
    m(3,3) = 1.0f;

    return m;
}

} // namespace ygl
