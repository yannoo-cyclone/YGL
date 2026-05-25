#include "math/vec2.hpp"

namespace ygl {

// Constructeurs
Vec2::Vec2() : x(0.0f), y(0.0f) {}
Vec2::Vec2(float scalar) : x(scalar), y(scalar) {}
Vec2::Vec2(float x, float y) : x(x), y(y) {}
Vec2::Vec2(const float* array) : x(array[0]), y(array[1]) {}

// Accès
float& Vec2::operator[](int index) { return data[index]; }
const float& Vec2::operator[](int index) const { return data[index]; }

// Opérateurs de modification
Vec2& Vec2::operator+=(const Vec2& other) { x += other.x; y += other.y; return *this; }
Vec2& Vec2::operator-=(const Vec2& other) { x -= other.x; y -= other.y; return *this; }
Vec2& Vec2::operator*=(float scalar) { x *= scalar; y *= scalar; return *this; }
Vec2& Vec2::operator/=(float scalar) { x /= scalar; y /= scalar; return *this; }

// Opérateurs arithmétiques
Vec2 Vec2::operator+(const Vec2& other) const { return Vec2(x + other.x, y + other.y); }
Vec2 Vec2::operator-(const Vec2& other) const { return Vec2(x - other.x, y - other.y); }
Vec2 Vec2::operator-() const { return Vec2(-x, -y); }
Vec2 Vec2::operator*(float scalar) const { return Vec2(x * scalar, y * scalar); }
Vec2 Vec2::operator/(float scalar) const { return Vec2(x / scalar, y / scalar); }

// Comparaison
bool Vec2::operator==(const Vec2& other) const { return x == other.x && y == other.y; }
bool Vec2::operator!=(const Vec2& other) const { return !(*this == other); }

// Calculs
float Vec2::length() const { return std::sqrt(x * x + y * y); }
float Vec2::lengthSquared() const { return x * x + y * y; }
Vec2 Vec2::normalized() const { float len = length(); return len > 0 ? *this / len : Vec2(); }
void Vec2::normalize() { *this = normalized(); }
float Vec2::dot(const Vec2& other) const { return x * other.x + y * other.y; }

// Méthodes statiques
Vec2 Vec2::zero() { return Vec2(0.0f); }
Vec2 Vec2::one() { return Vec2(1.0f); }
Vec2 Vec2::unitX() { return Vec2(1.0f, 0.0f); }
Vec2 Vec2::unitY() { return Vec2(0.0f, 1.0f); }

// Output
std::ostream& operator<<(std::ostream& os, const Vec2& v) {
    os << "Vec2(" << v.x << ", " << v.y << ")";
    return os;
}

// Opérateur non-member
Vec2 operator*(float scalar, const Vec2& v) {
    return v * scalar;
}

} // namespace ygl
