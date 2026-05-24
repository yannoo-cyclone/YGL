#include "math/vec2.hpp"
#include <cmath>
#include <algorithm>

namespace ygl {

// Assignment operators
Vec2& Vec2::operator+=(const Vec2& other) {
    x += other.x; y += other.y; return *this;
}
Vec2& Vec2::operator-=(const Vec2& other) {
    x -= other.x; y -= other.y; return *this;
}
Vec2& Vec2::operator*=(float scalar) {
    x *= scalar; y *= scalar; return *this;
}
Vec2& Vec2::operator/=(float scalar) {
    float inv = 1.0f / scalar; x *= inv; y *= inv; return *this;
}

// Arithmetic operators
Vec2 Vec2::operator+(const Vec2& other) const {
    return Vec2(x + other.x, y + other.y);
}
Vec2 Vec2::operator-(const Vec2& other) const {
    return Vec2(x - other.x, y - other.y);
}
Vec2 Vec2::operator-() const { return Vec2(-x, -y); }
Vec2 Vec2::operator*(float scalar) const {
    return Vec2(x * scalar, y * scalar);
}
Vec2 Vec2::operator/(float scalar) const {
    float inv = 1.0f / scalar; return Vec2(x * inv, y * inv);
}

// Comparison
bool Vec2::operator==(const Vec2& other) const {
    return x == other.x && y == other.y;
}
bool Vec2::operator!=(const Vec2& other) const {
    return !(*this == other);
}

// Vector operations
float Vec2::length() const { return std::sqrt(x*x + y*y); }
float Vec2::lengthSquared() const { return x*x + y*y; }
Vec2 Vec2::normalized() const {
    float len = length(); return len > 0.0f ? *this / len : *this;
}
void Vec2::normalize() { *this = normalized(); }
float Vec2::dot(const Vec2& other) const {
    return x*other.x + y*other.y;
}
float Vec2::distanceTo(const Vec2& other) const {
    return std::sqrt((x-other.x)*(x-other.x) + (y-other.y)*(y-other.y));
}
Vec2 Vec2::lerp(const Vec2& other, float t) const {
    return Vec2(x + (other.x-x)*t, y + (other.y-y)*t);
}

// Output
std::ostream& operator<<(std::ostream& os, const Vec2& v) {
    os << "Vec2(" << v.x << ", " << v.y << ")"; return os;
}

// Non-member functions
Vec2 operator*(float scalar, const Vec2& v) { return v * scalar; }
float dot(const Vec2& a, const Vec2& b) { return a.dot(b); }
Vec2 normalize(const Vec2& v) { return v.normalized(); }

} // namespace ygl
