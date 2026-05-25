#pragma once

#include <cstring>
#include <algorithm>
#include "math/vec3.hpp"
#include "math/vec4.hpp"
#include "math/quat.hpp"

namespace ygl {

class mat4 {
private:
    float data[16];

public:
    // Constructeurs
    mat4();
    explicit mat4(const float* values);

    // Accès
    float operator()(int row, int col) const;
    float& operator()(int row, int col);

    // Opérateurs
    mat4 operator*(const mat4& other) const;
    Vec3 operator*(const Vec3& v) const;
    Vec4 operator*(const Vec4& v) const;

    // Méthodes statiques
    static mat4 Identity();
    static mat4 Translation(const Vec3& vec);
    static mat4 Scale(const Vec3& vec);
    static mat4 RotationX(float angle);
    static mat4 RotationY(float angle);
    static mat4 RotationZ(float angle);
    static mat4 lookAt(const Vec3& eye, const Vec3& target, const Vec3& up);
    static mat4 perspective(float fov, float aspect, float near, float far);
    static mat4 orthographic(float left, float right, float bottom, float top, float near, float far);

    // Méthodes utilitaires
    mat4 inverted() const;
    float determinant() const;
    mat4 transposed() const;
};

} // namespace ygl
