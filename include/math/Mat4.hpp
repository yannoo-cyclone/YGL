#pragma once
#include <cstring>
#include <algorithm>

namespace ygl {
class Vec3; class Vec4; class Quat;
class Mat4 {
private: float data[16];
public:
    Mat4(); explicit Mat4(const float* values);
    float operator()(int row, int col) const; float& operator()(int row, int col);
    Mat4 operator*(const Mat4& other) const; Vec3 operator*(const Vec3& v) const; Vec4 operator*(const Vec4& v) const;
    static Mat4 Identity(); static Mat4 Translation(const Vec3& vec); static Mat4 Scale(const Vec3& vec);
    static Mat4 RotationX(float angle); static Mat4 RotationY(float angle); static Mat4 RotationZ(float angle);
    static Mat4 lookAt(const Vec3& eye, const Vec3& target, const Vec3& up);
    static Mat4 perspective(float fov, float aspect, float near, float far);
    static Mat4 orthographic(float left, float right, float bottom, float top, float near, float far);
    Mat4 inverted() const; float determinant() const; Mat4 transposed() const;
};
} // namespace ygl
