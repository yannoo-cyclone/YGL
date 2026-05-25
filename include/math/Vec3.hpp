#pragma once
#include <cmath>
#include <iostream>

namespace ygl {
class Mat4;
class Vec3 {
public:
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wpedantic"
    union { struct { float x, y, z; }; float data[3]; };
    #pragma GCC diagnostic pop

    Vec3(); explicit Vec3(float scalar); Vec3(float x, float y, float z);
    float& operator[](int index); const float& operator[](int index) const;
    Vec3& operator+=(const Vec3& other); Vec3& operator-=(const Vec3& other);
    Vec3& operator*=(float scalar); Vec3& operator/=(float scalar);
    Vec3 operator+(const Vec3& other) const; Vec3 operator-(const Vec3& other) const;
    Vec3 operator-() const; Vec3 operator*(float scalar) const; Vec3 operator/(float scalar) const;
    bool operator==(const Vec3& other) const; bool operator!=(const Vec3& other) const;
    float dot(const Vec3& other) const; Vec3 cross(const Vec3& other) const;
    float length() const; float lengthSquared() const;
    Vec3 normalized() const; void normalize();
    Vec3 Transform(const Mat4& matrix) const;
    static Vec3 zero(); static Vec3 one(); static Vec3 unitX(); static Vec3 unitY(); static Vec3 unitZ();
    friend std::ostream& operator<<(std::ostream& os, const Vec3& vec);
};
Vec3 operator*(float scalar, const Vec3& v);
Vec3 normalize(const Vec3& v); float dot(const Vec3& a, const Vec3& b); Vec3 cross(const Vec3& a, const Vec3& b);
} // namespace ygl
