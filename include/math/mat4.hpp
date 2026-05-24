#pragma once

#include <cstring> // Pour memcpy
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

    static mat4 Translation(const Vec3& vec);
    static mat4 Rotation(const Quat& quat);
    static mat4 Scale(const Vec3& vec);
};

} // namespace ygl
