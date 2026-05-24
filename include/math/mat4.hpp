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
    // Constructeurs
    mat4() {
        // Matrice identité
        for (int i = 0; i < 16; ++i) data[i] = (i % 5 == 0) ? 1.0f : 0.0f;
    }
    explicit mat4(const float* values) {
        std::memcpy(data, values, 16 * sizeof(float));
    }

    // Accès aux éléments (colonne-major)
    float operator()(int row, int col) const {
        return data[col * 4 + row];
    }
    float& operator()(int row, int col) {
        return data[col * 4 + row];
    }

    // Opérateurs
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

    // Méthodes utilitaires
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

    static mat4 lookAt(const Vec3& eye, const Vec3& target, const Vec3& up) {
        Vec3 forward = (target - eye).Normalize();
        Vec3 right = cross(forward, up).Normalize();
        Vec3 newUp = cross(right, forward);

        mat4 view;
        view(0, 0) = right.x; view(0, 1) = right.y; view(0, 2) = right.z; view(0, 3) = -dot(right, eye);
        view(1, 0) = newUp.x; view(1, 1) = newUp.y; view(1, 2) = newUp.z; view(1, 3) = -dot(newUp, eye);
        view(2, 0) = -forward.x; view(2, 1) = -forward.y; view(2, 2) = -forward.z; view(2, 3) = dot(forward, eye);
        view(3, 0) = 0.0f; view(3, 1) = 0.0f; view(3, 2) = 0.0f; view(3, 3) = 1.0f;
        return view;
    }

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

    mat4 inverted() const {
        // Implémentation simplifiée (à optimiser si nécessaire)
        float det = determinant();
        if (det == 0.0f) return mat4(); // Matrice singulière
        mat4 inv;
        inv.data[0] =  data[5]*data[10]*data[15] - data[5]*data[11]*data[14] - data[9]*data[6]*data[15] + data[9]*data[7]*data[14] + data[13]*data[6]*data[11] - data[13]*data[7]*data[10];
        inv.data[1] = -data[1]*data[10]*data[15] + data[1]*data[11]*data[14] + data[9]*data[2]*data[15] - data[9]*data[3]*data[14] - data[13]*data[2]*data[11] + data[13]*data[3]*data[10];
        inv.data[2] =  data[1]*data[6]*data[15] - data[1]*data[7]*data[14] - data[5]*data[2]*data[15] + data[5]*data[3]*data[14] + data[13]*data[2]*data[7] - data[13]*data[3]*data[6];
        inv.data[3] = -data[1]*data[6]*data[11] + data[1]*data[7]*data[10] + data[5]*data[2]*data[11] - data[5]*data[3]*data[10] - data[9]*data[2]*data[7] + data[9]*data[3]*data[6];
        inv.data[4] = -data[4]*data[10]*data[15] + data[4]*data[11]*data[14] + data[8]*data[6]*data[15] - data[8]*data[7]*data[14] - data[12]*data[6]*data[11] + data[12]*data[7]*data[10];
        inv.data[5] =  data[0]*data[10]*data[15] - data[0]*data[11]*data[14] - data[8]*data[2]*data[15] + data[8]*data[3]*data[14] + data[12]*data[2]*data[11] - data[12]*data[3]*data[10];
        inv.data[6] = -data[0]*data[6]*data[15] + data[0]*data[7]*data[14] + data[4]*data[2]*data[15] - data[4]*data[3]*data[14] - data[12]*data[2]*data[7] + data[12]*data[3]*data[6];
        inv.data[7] =  data[0]*data[6]*data[11] - data[0]*data[7]*data[10] - data[4]*data[2]*data[11] + data[4]*data[3]*data[10] + data[8]*data[2]*data[7] - data[8]*data[3]*data[6];
        inv.data[8] =  data[4]*data[9]*data[15] - data[4]*data[11]*data[13] - data[8]*data[5]*data[15] + data[8]*data[7]*data[13] + data[12]*data[5]*data[11] - data[12]*data[7]*data[9];
        inv.data[9] = -data[0]*data[9]*data[15] + data[0]*data[11]*data[13] + data[8]*data[1]*data[15] - data[8]*data[3]*data[13] - data[12]*data[1]*data[11] + data[12]*data[3]*data[9];
        inv.data[10] = data[0]*data[5]*data[15] - data[0]*data[7]*data[13] - data[4]*data[1]*data[15] + data[4]*data[3]*data[13] + data[12]*data[1]*data[7] - data[12]*data[3]*data[5];
        inv.data[11] = -data[0]*data[5]*data[11] + data[0]*data[7]*data[9] + data[4]*data[1]*data[11] - data[4]*data[3]*data[9] - data[8]*data[1]*data[7] + data[8]*data[3]*data[5];
        inv.data[12] = -data[4]*data[9]*data[14] + data[4]*data[10]*data[13] + data[8]*data[5]*data[14] - data[8]*data[6]*data[13] - data[12]*data[5]*data[10] + data[12]*data[6]*data[9];
        inv.data[13] = data[0]*data[9]*data[14] - data[0]*data[10]*data[13] - data[8]*data[1]*data[14] + data[8]*data[2]*data[13] + data[12]*data[1]*data[10] - data[12]*data[2]*data[9];
        inv.data[14] = -data[0]*data[5]*data[14] + data[0]*data[6]*data[13] + data[4]*data[1]*data[14] - data[4]*data[2]*data[13] - data[12]*data[1]*data[6] + data[12]*data[2]*data[5];
        inv.data[15] = data[0]*data[5]*data[10] - data[0]*data[6]*data[9] - data[4]*data[1]*data[10] + data[4]*data[2]*data[9] + data[8]*data[1]*data[6] - data[8]*data[2]*data[5];
        float invDet = 1.0f / det;
        for (int i = 0; i < 16; ++i) inv.data[i] *= invDet;
        return inv;
    }

    mat4 transposed() const {
        mat4 t;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                t(i, j) = data[j * 4 + i];
            }
        }
        return t;
    }

    float determinant() const {
        return data[0]*data[5]*data[10]*data[15] - data[0]*data[5]*data[11]*data[14] - data[0]*data[9]*data[6]*data[15] + data[0]*data[9]*data[7]*data[14]
             + data[4]*data[1]*data[10]*data[15] - data[4]*data[1]*data[11]*data[14] - data[4]*data[9]*data[2]*data[15] + data[4]*data[9]*data[3]*data[14]
             + data[8]*data[1]*data[6]*data[15] - data[8]*data[1]*data[7]*data[14] - data[8]*data[5]*data[2]*data[15] + data[8]*data[5]*data[3]*data[14]
             - data[12]*data[1]*data[6]*data[11] + data[12]*data[1]*data[7]*data[10] + data[12]*data[5]*data[2]*data[9] - data[12]*data[5]*data[3]*data[8];
    }
};

} // namespace ygl
