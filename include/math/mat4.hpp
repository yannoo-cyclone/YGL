#pragma once

#include "vec3.hpp"
#include "vec4.hpp"
#include <iostream>

namespace ygl {

class Mat4 {
public:
    union {
        float data[16];
        float m[4][4];
    };

    Mat4();
    Mat4(float diagonal);
    Mat4(const float* array);
    Mat4(const Vec4& col0, const Vec4& col1, const Vec4& col2, const Vec4& col3);

    float& operator()(int row, int col);
    const float& operator()(int row, int col) const;

    Mat4& operator+=(const Mat4& other);
    Mat4& operator-=(const Mat4& other);
    Mat4& operator*=(const Mat4& other);
    Mat4& operator*=(float scalar);
    Mat4& operator/=(float scalar);

    Mat4 operator+(const Mat4& other) const;
    Mat4 operator-(const Mat4& other) const;
    Mat4 operator-() const;
    Mat4 operator*(const Mat4& other) const;
    Mat4 operator*(float scalar) const;
    Mat4 operator/(float scalar) const;

    Vec4 operator*(const Vec4& v) const;
    Vec3 operator*(const Vec3& v) const;

    bool operator==(const Mat4& other) const;
    bool operator!=(const Mat4& other) const;

    Mat4 transposed() const;
    void transpose();
    Mat4 inverted() const;
    void invert();
    float determinant() const;

    static Mat4 identity();
    static Mat4 zero();
    static Mat4 translation(const Vec3& t);
    static Mat4 rotationX(float angle);
    static Mat4 rotationY(float angle);
    static Mat4 rotationZ(float angle);
    static Mat4 rotation(const Vec3& axis, float angle);
    static Mat4 scale(const Vec3& s);
    static Mat4 scale(float s);
    static Mat4 lookAt(const Vec3& eye, const Vec3& center, const Vec3& up);
    static Mat4 perspective(float fov, float aspect, float near, float far);
    static Mat4 orthographic(float left, float right, float bottom, float top, float near, float far);

    friend std::ostream& operator<<(std::ostream& os, const Mat4& m);
};

Mat4 operator*(float scalar, const Mat4& m);
Mat4 transpose(const Mat4& m);
Mat4 invert(const Mat4& m);

} // namespace ygl

