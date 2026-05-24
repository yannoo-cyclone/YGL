#include "math/mat4.hpp"
#include "math/vec3.hpp"
#include "math/vec4.hpp"
#include <cmath>
#include <cstring>
#include <algorithm>
#include <ostream>

namespace ygl {

// Constructors
mat4::mat4() { *this = identity(); }
mat4::mat4(float diagonal) {
    data[0] = data[5] = data[10] = data[15] = diagonal;
    data[1] = data[2] = data[3] = data[4] = data[6] = data[7] =
    data[8] = data[9] = data[11] = data[12] = data[13] = data[14] = 0.0f;
}
mat4::mat4(const float* array) {
    std::memcpy(data, array, 16 * sizeof(float));
}
mat4::mat4(const Vec4& col0, const Vec4& col1, const Vec4& col2, const Vec4& col3) {
    for (int i = 0; i < 4; ++i) {
        data[i] = col0.data[i];
        data[i+4] = col1.data[i];
        data[i+8] = col2.data[i];
        data[i+12] = col3.data[i];
    }
}

// Access
float& mat4::operator()(int row, int col) { return data[col * 4 + row]; }
const float& mat4::operator()(int row, int col) const { return data[col * 4 + row]; }

// Assignment
mat4& mat4::operator+=(const mat4& other) {
    for (int i = 0; i < 16; ++i) data[i] += other.data[i]; return *this;
}
mat4& mat4::operator-=(const mat4& other) {
    for (int i = 0; i < 16; ++i) data[i] -= other.data[i]; return *this;
}
mat4& mat4::operator*=(const mat4& other) { *this = *this * other; return *this; }
mat4& mat4::operator*=(float scalar) {
    for (int i = 0; i < 16; ++i) data[i] *= scalar; return *this;
}
mat4& mat4::operator/=(float scalar) {
    float inv = 1.0f / scalar;
    for (int i = 0; i < 16; ++i) data[i] *= inv; return *this;
}

// Arithmetic
mat4 mat4::operator+(const mat4& other) const {
    mat4 result; for (int i = 0; i < 16; ++i) result.data[i] = data[i] + other.data[i]; return result;
}
mat4 mat4::operator-(const mat4& other) const {
    mat4 result; for (int i = 0; i < 16; ++i) result.data[i] = data[i] - other.data[i]; return result;
}
mat4 mat4::operator-() const {
    mat4 result; for (int i = 0; i < 16; ++i) result.data[i] = -data[i]; return result;
}
mat4 mat4::operator*(const mat4& other) const {
    mat4 result;
    for (int col = 0; col < 4; ++col) {
        for (int row = 0; row < 4; ++row) {
            result(row, col) = 0.0f;
            for (int k = 0; k < 4; ++k) {
                result(row, col) += (*this)(row, k) * other(k, col);
            }
        }
    }
    return result;
}
mat4 mat4::operator*(float scalar) const {
    mat4 result; for (int i = 0; i < 16; ++i) result.data[i] = data[i] * scalar; return result;
}
mat4 mat4::operator/(float scalar) const {
    float inv = 1.0f / scalar;
    mat4 result; for (int i = 0; i < 16; ++i) result.data[i] = data[i] * inv; return result;
}

Vec4 mat4::operator*(const Vec4& v) const {
    return Vec4(
        (*this)(0,0)*v.x + (*this)(0,1)*v.y + (*this)(0,2)*v.z + (*this)(0,3)*v.w,
        (*this)(1,0)*v.x + (*this)(1,1)*v.y + (*this)(1,2)*v.z + (*this)(1,3)*v.w,
        (*this)(2,0)*v.x + (*this)(2,1)*v.y + (*this)(2,2)*v.z + (*this)(2,3)*v.w,
        (*this)(3,0)*v.x + (*this)(3,1)*v.y + (*this)(3,2)*v.z + (*this)(3,3)*v.w
    );
}

Vec3 mat4::operator*(const Vec3& v) const {
    Vec4 v4(v.x, v.y, v.z, 1.0f); Vec4 result = *this * v4;
    return Vec3(result.x, result.y, result.z);
}

// Comparison
bool mat4::operator==(const mat4& other) const {
    for (int i = 0; i < 16; ++i) if (data[i] != other.data[i]) return false;
    return true;
}
bool mat4::operator!=(const mat4& other) const { return !(*this == other); }

// Matrix operations
mat4 mat4::transposed() const {
    mat4 result;
    for (int row = 0; row < 4; ++row)
        for (int col = 0; col < 4; ++col)
            result(row, col) = (*this)(col, row);
    return result;
}
void mat4::transpose() { *this = transposed(); }

float mat4::determinant() const {
    float det = 0.0f, sign = 1.0f;
    for (int col = 0; col < 4; ++col) {
        float submatrix[9]; int subIndex = 0;
        for (int row = 1; row < 4; ++row) {
            for (int c = 0; c < 4; ++c) {
                if (c == col) continue;
                submatrix[subIndex++] = (*this)(row, c);
            }
        }
        float subDet = submatrix[0]*(submatrix[4]*submatrix[8]-submatrix[5]*submatrix[7]) -
                       submatrix[1]*(submatrix[3]*submatrix[8]-submatrix[5]*submatrix[6]) +
                       submatrix[2]*(submatrix[3]*submatrix[7]-submatrix[4]*submatrix[6]);
        det += sign * (*this)(0, col) * subDet;
        sign = -sign;
    }
    return det;
}

mat4 mat4::inverted() const {
    float det = determinant();
    if (std::abs(det) < 1e-6f) return identity();
    float invDet = 1.0f / det; mat4 adjugate;
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            float submatrix[9]; int subIndex = 0;
            for (int r = 0; r < 4; ++r) {
                if (r == row) continue;
                for (int c = 0; c < 4; ++c) {
                    if (c == col) continue;
                    submatrix[subIndex++] = (*this)(r, c);
                }
            }
            float cofactor = submatrix[0]*(submatrix[4]*submatrix[8]-submatrix[5]*submatrix[7]) -
                            submatrix[1]*(submatrix[3]*submatrix[8]-submatrix[5]*submatrix[6]) +
                            submatrix[2]*(submatrix[3]*submatrix[7]-submatrix[4]*submatrix[6]);
            if ((row+col) % 2 == 1) cofactor = -cofactor;
            adjugate(col, row) = cofactor;
        }
    }
    return adjugate * invDet;
}
void mat4::invert() { *this = inverted(); }

// Static
mat4 mat4::identity() { mat4 m; for (int i = 0; i < 16; i += 5) m.data[i] = 1.0f; return m; }
mat4 mat4::zero() { mat4 m; for (int i = 0; i < 16; ++i) m.data[i] = 0.0f; return m; }

mat4 mat4::translation(const Vec3& t) {
    mat4 m = identity(); m(0,3) = t.x; m(1,3) = t.y; m(2,3) = t.z; return m;
}

mat4 mat4::rotationX(float angle) {
    float c = std::cos(angle), s = std::sin(angle);
    mat4 m = identity(); m(1,1) = c; m(1,2) = -s; m(2,1) = s; m(2,2) = c; return m;
}
mat4 mat4::rotationY(float angle) {
    float c = std::cos(angle), s = std::sin(angle);
    mat4 m = identity(); m(0,0) = c; m(0,2) = s; m(2,0) = -s; m(2,2) = c; return m;
}
mat4 mat4::rotationZ(float angle) {
    float c = std::cos(angle), s = std::sin(angle);
    mat4 m = identity(); m(0,0) = c; m(0,1) = -s; m(1,0) = s; m(1,1) = c; return m;
}

mat4 mat4::rotation(const Vec3& axis, float angle) {
    Vec3 u = normalize(axis); float c = std::cos(angle), s = std::sin(angle);
    mat4 m;
    m(0,0) = c + u.x*u.x*(1-c); m(0,1) = u.x*u.y*(1-c) - u.z*s; m(0,2) = u.x*u.z*(1-c) + u.y*s;
    m(1,0) = u.y*u.x*(1-c) + u.z*s; m(1,1) = c + u.y*u.y*(1-c); m(1,2) = u.y*u.z*(1-c) - u.x*s;
    m(2,0) = u.z*u.x*(1-c) - u.y*s; m(2,1) = u.z*u.y*(1-c) + u.x*s; m(2,2) = c + u.z*u.z*(1-c);
    m(3,3) = 1.0f; return m;
}

mat4 mat4::scale(const Vec3& s) {
    mat4 m = identity(); m(0,0) = s.x; m(1,1) = s.y; m(2,2) = s.z; return m;
}
mat4 mat4::scale(float s) { return scale(Vec3(s)); }

mat4 mat4::lookAt(const Vec3& eye, const Vec3& center, const Vec3& up) {
    Vec3 f = normalize(center - eye); Vec3 s = normalize(cross(f, up)); Vec3 u = cross(s, f);
    mat4 m = identity();
    m(0,0) = s.x; m(0,1) = s.y; m(0,2) = s.z;
    m(1,0) = u.x; m(1,1) = u.y; m(1,2) = u.z;
    m(2,0) = -f.x; m(2,1) = -f.y; m(2,2) = -f.z;
    m(0,3) = -dot(s, eye); m(1,3) = -dot(u, eye); m(2,3) = dot(f, eye);
    return m;
}

mat4 mat4::perspective(float fov, float aspect, float near, float far) {
    float tanHalfFov = std::tan(fov * 0.5f);
    mat4 m = zero();
    m(0,0) = 1.0f / (aspect * tanHalfFov);
    m(1,1) = 1.0f / tanHalfFov;
    m(2,2) = -(far + near) / (far - near);
    m(2,3) = -1.0f;
    m(3,2) = -(2.0f * far * near) / (far - near);
    return m;
}

mat4 mat4::orthographic(float left, float right, float bottom, float top, float near, float far) {
    mat4 m = identity();
    m(0,0) = 2.0f / (right - left);
    m(1,1) = 2.0f / (top - bottom);
    m(2,2) = -2.0f / (far - near);
    m(0,3) = -(right + left) / (right - left);
    m(1,3) = -(top + bottom) / (top - bottom);
    m(2,3) = -(far + near) / (far - near);
    return m;
}

// Output
std::ostream& operator<<(std::ostream& os, const mat4& m) {
    os << "Mat4([\n";
    for (int i = 0; i < 4; ++i) {
        if (i > 0) os << "        ";
        for (int j = 0; j < 4; ++j) {
            if (j > 0) os << ", ";
            os << m(i, j);
        }
        if (i < 3) os << ",\n";
    }
    os << "])"; return os;
}

// Non-member
mat4 operator*(float scalar, const mat4& m) { return m * scalar; }
mat4 transpose(const mat4& m) { return m.transposed(); }
mat4 invert(const mat4& m) { return m.inverted(); }

} // namespace ygl
