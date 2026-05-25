#include "math/mat4.hpp"

namespace ygl {

// Constructeurs
mat4::mat4() {
    for (int i = 0; i < 16; ++i) {
        data[i] = (i % 5 == 0) ? 1.0f : 0.0f;
    }
}

mat4::mat4(const float* values) {
    std::memcpy(data, values, 16 * sizeof(float));
}

// Accès
float mat4::operator()(int row, int col) const {
    return data[col * 4 + row];
}

float& mat4::operator()(int row, int col) {
    return data[col * 4 + row];
}

// Opérateurs
mat4 mat4::operator*(const mat4& other) const {
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

Vec3 mat4::operator*(const Vec3& v) const {
    float w = data[3]*v.x + data[7]*v.y + data[11]*v.z + data[15];
    return Vec3(
        (data[0]*v.x + data[4]*v.y + data[8]*v.z + data[12]) / w,
        (data[1]*v.x + data[5]*v.y + data[9]*v.z + data[13]) / w,
        (data[2]*v.x + data[6]*v.y + data[10]*v.z + data[14]) / w
    );
}

Vec4 mat4::operator*(const Vec4& v) const {
    return Vec4(
        data[0]*v.x + data[4]*v.y + data[8]*v.z + data[12]*v.w,
        data[1]*v.x + data[5]*v.y + data[9]*v.z + data[13]*v.w,
        data[2]*v.x + data[6]*v.y + data[10]*v.z + data[14]*v.w,
        data[3]*v.x + data[7]*v.y + data[11]*v.z + data[15]*v.w
    );
}

// Méthodes statiques
mat4 mat4::Identity() { return mat4(); }

mat4 mat4::Translation(const Vec3& vec) {
    mat4 m;
    m(0, 3) = vec.x;
    m(1, 3) = vec.y;
    m(2, 3) = vec.z;
    return m;
}

mat4 mat4::Scale(const Vec3& vec) {
    mat4 m;
    m(0, 0) = vec.x;
    m(1, 1) = vec.y;
    m(2, 2) = vec.z;
    return m;
}

mat4 mat4::RotationX(float angle) {
    float c = std::cos(angle);
    float s = std::sin(angle);
    mat4 m;
    m(1, 1) = c; m(1, 2) = -s;
    m(2, 1) = s; m(2, 2) = c;
    return m;
}

mat4 mat4::RotationY(float angle) {
    float c = std::cos(angle);
    float s = std::sin(angle);
    mat4 m;
    m(0, 0) = c; m(0, 2) = s;
    m(2, 0) = -s; m(2, 2) = c;
    return m;
}

mat4 mat4::RotationZ(float angle) {
    float c = std::cos(angle);
    float s = std::sin(angle);
    mat4 m;
    m(0, 0) = c; m(0, 1) = -s;
    m(1, 0) = s; m(1, 1) = c;
    return m;
}

mat4 mat4::lookAt(const Vec3& eye, const Vec3& target, const Vec3& up) {
    Vec3 forward = (target - eye).normalized();
    Vec3 right = cross(forward, up).normalized();
    Vec3 newUp = cross(right, forward);

    mat4 view;
    view(0, 0) = right.x; view(0, 1) = right.y; view(0, 2) = right.z; view(0, 3) = -right.dot(eye);
    view(1, 0) = newUp.x; view(1, 1) = newUp.y; view(1, 2) = newUp.z; view(1, 3) = -newUp.dot(eye);
    view(2, 0) = -forward.x; view(2, 1) = -forward.y; view(2, 2) = -forward.z; view(2, 3) = forward.dot(eye);
    view(3, 0) = 0.0f; view(3, 1) = 0.0f; view(3, 2) = 0.0f; view(3, 3) = 1.0f;
    return view;
}

mat4 mat4::perspective(float fov, float aspect, float near, float far) {
    float tanHalfFov = std::tan(fov / 2.0f);
    mat4 m;
    m(0, 0) = 1.0f / (aspect * tanHalfFov);
    m(1, 1) = 1.0f / tanHalfFov;
    m(2, 2) = -(far + near) / (far - near);
    m(2, 3) = -1.0f;
    m(3, 2) = -(2.0f * far * near) / (far - near);
    return m;
}

mat4 mat4::orthographic(float left, float right, float bottom, float top, float near, float far) {
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

// Méthodes utilitaires
mat4 mat4::inverted() const {
    float det = determinant();
    if (det == 0.0f) return mat4();

    mat4 inv;
    inv.data[0] =  data[5]*data[10]*data[15] - data[5]*data[11]*data[14] - data[9]*data[6]*data[15] + data[9]*data[7]*data[14] + data[13]*data[6]*data[11] - data[13]*data[7]*data[10];
    inv.data[4] = -data[4]*data[10]*data[15] + data[4]*data[11]*data[14] + data[8]*data[6]*data[15] - data[8]*data[7]*data[14] - data[12]*data[6]*data[11] + data[12]*data[7]*data[10];
    inv.data[8] =  data[4]*data[9]*data[15] - data[4]*data[11]*data[13] - data[8]*data[5]*data[15] + data[8]*data[7]*data[13] + data[12]*data[5]*data[11] - data[12]*data[7]*data[9];
    inv.data[12] = -data[4]*data[9]*data[14] + data[4]*data[11]*data[13] + data[8]*data[5]*data[14] - data[8]*data[6]*data[13] - data[12]*data[5]*data[10] + data[12]*data[6]*data[9];

    inv.data[1] = -data[1]*data[10]*data[15] + data[1]*data[11]*data[14] + data[9]*data[2]*data[15] - data[9]*data[3]*data[14] - data[13]*data[2]*data[11] + data[13]*data[3]*data[10];
    inv.data[5] =  data[0]*data[10]*data[15] - data[0]*data[11]*data[14] - data[8]*data[2]*data[15] + data[8]*data[3]*data[14] + data[12]*data[2]*data[11] - data[12]*data[3]*data[10];
    inv.data[9] = -data[0]*data[9]*data[15] + data[0]*data[11]*data[13] + data[8]*data[1]*data[15] - data[8]*data[3]*data[13] - data[12]*data[1]*data[11] + data[12]*data[3]*data[9];
    inv.data[13] =  data[0]*data[9]*data[14] - data[0]*data[11]*data[13] - data[8]*data[1]*data[14] + data[8]*data[2]*data[13] + data[12]*data[1]*data[7] - data[12]*data[2]*data[9];

    inv.data[2] =  data[1]*data[6]*data[15] - data[1]*data[7]*data[14] - data[5]*data[2]*data[15] + data[5]*data[3]*data[14] + data[13]*data[2]*data[7] - data[13]*data[3]*data[6];
    inv.data[6] = -data[0]*data[6]*data[15] + data[0]*data[7]*data[14] + data[4]*data[2]*data[15] - data[4]*data[3]*data[14] - data[12]*data[2]*data[7] + data[12]*data[3]*data[6];
    inv.data[10] =  data[0]*data[5]*data[15] - data[0]*data[7]*data[13] - data[4]*data[1]*data[15] + data[4]*data[3]*data[13] + data[12]*data[1]*data[7] - data[12]*data[3]*data[5];
    inv.data[14] = -data[0]*data[5]*data[14] + data[0]*data[7]*data[13] + data[4]*data[1]*data[14] - data[4]*data[3]*data[13] - data[12]*data[1]*data[6] + data[12]*data[2]*data[5];

    inv.data[3] = -data[1]*data[6]*data[11] + data[1]*data[7]*data[10] + data[5]*data[2]*data[11] - data[5]*data[3]*data[10] - data[9]*data[2]*data[7] + data[9]*data[3]*data[6];
    inv.data[7] =  data[0]*data[6]*data[11] - data[0]*data[7]*data[10] - data[4]*data[2]*data[11] + data[4]*data[3]*data[10] + data[8]*data[2]*data[7] - data[8]*data[3]*data[6];
    inv.data[11] = -data[0]*data[5]*data[11] + data[0]*data[7]*data[9] + data[4]*data[1]*data[11] - data[4]*data[3]*data[9] - data[8]*data[1]*data[7] + data[8]*data[3]*data[5];
    inv
