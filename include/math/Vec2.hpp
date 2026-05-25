#pragma once
#include <cmath>
#include <iostream>

namespace ygl {
class Vec2 {
public:
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wpedantic"
    union { struct { float x, y; }; struct { float u, v; }; float data[2]; };
    #pragma GCC diagnostic pop

    Vec2(); explicit Vec2(float scalar); Vec2(float x, float y); Vec2(const float* array);
    float& operator[](int index); const float& operator[](int index) const;
    Vec2& operator+=(const Vec2& other); Vec2& operator-=(const Vec2& other);
    Vec2& operator*=(float scalar); Vec2& operator/=(float scalar);
    Vec2 operator+(const Vec2& other) const; Vec2 operator-(const Vec2& other) const;
    Vec2 operator-() const; Vec2 operator*(float scalar) const; Vec2 operator/(float scalar) const;
    bool operator==(const Vec2& other) const; bool operator!=(const Vec2& other) const;
    float length() const; float lengthSquared() const;
    Vec2 normalized() const; void normalize(); float dot(const Vec2& other) const;
    static Vec2 zero(); static Vec2 one(); static Vec2 unitX(); static Vec2 unitY();
    friend std::ostream& operator<<(std::ostream& os, const Vec2& v);
};
Vec2 operator*(float scalar, const Vec2& v);
} // namespace ygl
