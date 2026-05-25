#include "math/Vec4.hpp"
#include "math/Vec3.hpp"
namespace ygl {
Vec4::Vec4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
Vec4::Vec4(float scalar) : x(scalar), y(scalar), z(scalar), w(scalar) {}
Vec4::Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
Vec4::Vec4(const Vec3& v, float w) : x(v.x), y(v.y), z(v.z), w(w) {}
Vec4& Vec4::operator+=(const Vec4& other) { x += other.x; y += other.y; z += other.z; w += other.w; return *this; }
Vec4& Vec4::operator-=(const Vec4& other) { x -= other.x; y -= other.y; z -= other.z; w -= other.w; return *this; }
Vec4& Vec4::operator*=(float scalar) { x *= scalar; y *= scalar; z *= scalar; w *= scalar; return *this; }
Vec4& Vec4::operator/=(float scalar) { x /= scalar; y /= scalar; z /= scalar; w /= scalar; return *this; }
Vec4 Vec4::operator+(const Vec4& other) const { return Vec4(x + other.x, y + other.y, z + other.z, w + other.w); }
Vec4 Vec4::operator-(const Vec4& other) const { return Vec4(x - other.x, y - other.y, z - other.z, w - other.w); }
Vec4 Vec4::operator-() const { return Vec4(-x, -y, -z, -w); }
Vec4 Vec4::operator*(float scalar) const { return Vec4(x * scalar, y * scalar, z * scalar, w * scalar); }
Vec4 Vec4::operator/(float scalar) const { return Vec4(x / scalar, y / scalar, z / scalar, w / scalar); }
bool Vec4::operator==(const Vec4& other) const { return x == other.x && y == other.y && z == other.z && w == other.w; }
bool Vec4::operator!=(const Vec4& other) const { return !(*this == other); }
float Vec4::length() const { return std::sqrt(x * x + y * y + z * z + w * w); }
float Vec4::lengthSquared() const { return x * x + y * y + z * z + w * w; }
Vec4 Vec4::normalized() const { float len = length(); return len > 0 ? *this / len : Vec4(); }
void Vec4::normalize() { *this = normalized(); }
float Vec4::dot(const Vec4& other) const { return x * other.x + y * other.y + z * other.z + w * other.w; }
Vec4 Vec4::zero() { return Vec4(0.0f); }
Vec4 Vec4::one() { return Vec4(1.0f); }
std::ostream& operator<<(std::ostream& os, const Vec4& v) { os << "Vec4(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")"; return os; }
Vec4 operator*(float scalar, const Vec4& v) { return v * scalar; }
float dot(const Vec4& a, const Vec4& b) { return a.dot(b); }
Vec4 normalize(const Vec4& v) { return v.normalized(); }
} // namespace ygl
