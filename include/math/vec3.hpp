#include "math/vec3.hpp"
#include "math/mat4.hpp"

namespace ygl {

// Constructeurs
Vec3::Vec3() : x(0), y(0), z(0) {}
Vec3::Vec3(float scalar) : x(scalar), y(scalar), z(scalar) {}
Vec3::Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

// Opérateurs de modification
Vec3& Vec3::operator+=(const Vec3& other) { x += other.x; y += other.y; z += other.z; return *this; }
Vec3& Vec3::operator-=(const Vec3& other) { x -= other.x; y -= other.y; z -= other.z; return *this; }
Vec3& Vec3::operator*=(float scalar) { x *= scalar; y *= scalar; z *= scalar; return *this; }
Vec3& Vec3::operator/=(float scalar) { float inv = 1.0f / scalar; x *= inv; y *= inv; z *= inv; return *this; }

// Opérateurs arithmétiques
Vec3 Vec3::operator+(const Vec3& other) const { return Vec3(x + other.x, y + other.y, z + other.z); }
Vec3 Vec3::operator-(const Vec3& other) const { return Vec3(x - other.x, y - other.y, z - other.z); }
Vec3 Vec3::operator-() const { return Vec3(-x, -y, -z); }
Vec3 Vec3::operator*(float scalar) const { return Vec3(x * scalar, y * scalar, z * scalar); }
Vec3 Vec3::operator/(float scalar) const { return Vec3(x / scalar, y / scalar, z / scalar); }

// Comparaison
bool Vec3::operator==(const Vec3& other) const { return x == other.x && y == other.y && z == other.z; }
bool Vec3::operator!=(const Vec3& other) const { return !(*this == other); }

// Calculs
float Vec3::dot(const Vec3& other) const { return x * other.x + y * other.y + z * other.z; }
Vec3 Vec3::cross(const Vec3& other) const {
    return Vec3(
        y * other.z - z * other.y,
        z * other.x - x * other.z,
        x * other.y - y * other.x
    );
}
float Vec3::length() const { return std::sqrt(x * x + y * y + z * z); }
float Vec3::lengthSquared() const { return x * x + y * y + z * z; }
Vec3 Vec3::normalized() const { float len = length(); return len > 0 ? *this / len : Vec3(); }
void Vec3::normalize() { *this = normalized(); }

// Transformation
Vec3 Vec3::Transform(const mat4& matrix) const {
    Vec3 result;
    result.x = matrix(0,0)*x + matrix(0,1)*y + matrix(0,2)*z + matrix(0,3);
    result.y = matrix(1,0)*x + matrix(1,1)*y + matrix(1,2)*z + matrix(1,3);
    result.z = matrix(2,0)*x + matrix(2,1)*y + matrix(2,2)*z + matrix(2,3);
    float w = matrix(3,0)*x + matrix(3,1)*y + matrix(3,2)*z + matrix(3,3);
    if (w != 0.0f && w != 1.0f) {
        result /= w;
    }
    return result;
}

// Méthodes statiques
Vec3 Vec3::zero() { return Vec3(0.0f); }
Vec3 Vec3::one() { return Vec3(1.0f); }
Vec3 Vec3::unitX() { return Vec3(1.0f, 0.0f, 0.0f); }
Vec3 Vec3::unitY() { return Vec3(0.0f, 1.0f, 0.0f); }
Vec3 Vec3::unitZ() { return Vec3(0.0f, 0.0f, 1.0f); }

// Output
std::ostream& operator<<(std::ostream& os, const Vec3& vec) {
    os << "Vec3(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
    return os;
}

// Fonctions non-member
Vec3 operator*(float scalar, const Vec3& v) { return v * scalar; }
Vec3 normalize(const Vec3& v) { return v.normalized(); }
float dot(const Vec3& a, const Vec3& b) { return a.dot(b); }
Vec3 cross(const Vec3& a, const Vec3& b) { return a.cross(b); }

} // namespace ygl
