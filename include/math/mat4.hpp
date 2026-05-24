#pragma once

#include <cstring>
#include <algorithm>

namespace ygl {

class Vec3;
class Vec4;
class Quat;

class mat4 {
private:
    float data[16]; // Stockage en colonne-major (standard OpenGL)

public:
    // ======================
    // Constructeurs
    // ======================
    mat4() {
        // Matrice identité
        for (int i = 0; i < 16; ++i) {
            data[i] = (i % 5 == 0) ? 1.0f : 0.0f;
        }
    }

    explicit mat4(const float* values) {
        std::memcpy(data, values, 16 * sizeof(float));
    }

    // ======================
    // Accès aux éléments
    // ======================
    float operator()(int row, int col) const {
        return data[col * 4 + row];
    }

    float& operator()(int row, int col) {
        return data[col * 4 + row];
    }

    // ======================
    // Opérateurs
    // ======================
    mat4 operator*(const mat4& other) const {
        mat4 result;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                result(i, j) = 0;
                for (int k = 0; k < 4; ++k) {
                    result(i, j) += data[k * 4 + i] * other.data[j * 4 + k];
                }
            }
        }
        return result;
    }

    // Multiplication matrice-vecteur (Vec3) - pour les points (w=1)
    Vec3 operator*(const Vec3& v) const;

    // Multiplication matrice-vecteur (Vec4) - pour les vecteurs homogènes
    Vec4 operator*(const Vec4& v) const {
        return Vec4(
            data[0]*v.x + data[4]*v.y + data[8]*v.z + data[12]*v.w,
            data[1]*v.x + data[5]*v.y + data[9]*v.z + data[13]*v.w,
            data[2]*v.x + data[6]*v.y + data[10]*v.z + data[14]*v.w,
            data[3]*v.x + data[7]*v.y + data[11]*v.z + data[15]*v.w
        );
    }

    // ======================
    // Méthodes utilitaires
    // ======================
    static mat4 Identity() {
        return mat4();
    }

    static mat4 Translation(const Vec3& vec) {
        mat4 m;
        m(0, 3) = vec.x;
        m(1, 3) = vec.y;
        m(2, 3) = vec.z;
        return m;
    }

    static mat4 Scale(const Vec3& vec) {
        mat4 m;
        m(0, 0) = vec.x;
        m(1, 1) = vec.y;
        m(2, 2) = vec.z;
        return m;
    }

    static mat4 RotationX(float angle) {
        float c = std::cos(angle);
        float s = std::sin(angle);
        mat4 m;
        m(1, 1) = c; m(1, 2) = -s;
        m(2, 1) = s; m(2, 2) = c;
        return m;
    }

    static mat4 RotationY(float angle) {
        float c = std::cos(angle);
        float s = std::sin(angle);
        mat4 m;
        m(0, 0) = c; m(0, 2) = s;
        m(2, 0) = -s; m(2, 2) = c;
        return m;
    }

    static mat4 RotationZ(float angle) {
        float c = std::cos(angle);
        float s = std::sin(angle);
        mat4 m;
        m(0, 0) = c; m(0, 1) = -s;
        m(1, 0) = s; m(1, 1) = c;
        return m;
    }

    static mat4 lookAt(const Vec3& eye, const Vec3& target, const Vec3& up);

    static mat4 perspective(float fov, float aspect, float near, float far) {
        float tanHalfFov = std::tan(fov / 2.0f);
        mat4 m;
        m(0, 0) = 1.0f / (aspect * tanHalfFov);
        m(1, 1) = 1.0f / tanHalfFov;
        m(2, 2) = -(far + near) / (far - near);
        m(2, 3) = -1.0f;
        m(3, 2) = -(2.0f * far * near) / (far - near);
        return m;
    }

    static mat4 orthographic(float left, float right, float bottom, float top, float near, float far) {
        mat4 m;
        m(0, 0) = 2.0f / (right - left);
        m(1, 1) = 2.0f / (top - bottom);
        m(2, 2) = -2.0f / (far - near);
        m(0, 3) = -(right + left) / (right - left);
        m(1, 3) = -(top + bottom) / (top - bottom);
        m(2, 3) = -(far + near) / (far - near);
        m(3, 3) = 1.0f;
        return m;
    }

    mat4 inverted() const;
    float determinant() const;
    mat4 transposed() const;
};

} // namespace ygl
