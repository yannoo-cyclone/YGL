#include "math/quat.hpp"
#include <cmath>
#include <algorithm>

namespace ygl {

// Constructors
Quat::Quat() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}
Quat::Quat(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

Quat::Quat(const Vec3& axis, float angle) {
    Vec3 a = normalize(axis);
    float halfAngle = angle * 0.5f;
    float sinHalf = std::sin(halfAngle);
    x = a.x * sinHalf;
    y = a.y * sinHalf;
    z = a.z * sinHalf;
    w = std::cos(halfAngle);
}

Quat::Quat(const Vec4& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}

Quat::Quat(const Mat4& m) {
    float trace = m(0,0) + m(1,1) + m(2,2);
    if (trace > 0.0f) {
        float s = 0.5f / std::sqrt(trace + 1.0f);
        w = 0.25f / s;
        x = (m(2,1) - m(1,2)) * s;
        y = (m(0,2) - m(2,0)) * s;
        z = (m(1,0) - m(0,1)) * s;
    } else {
        if (m(0,0) > m(1,1) && m(0,0) > m(2,2)) {
            float s = 2.0f * std::sqrt(1.0f + m(0,0) - m(1,1) - m(2,2));
            w = (m(2,1) - m(1,2)) / s;
            x = 0.25f * s;
            y = (m(0,1) + m(1,0)) / s;
            z = (m(0,2) + m(2,0)) / s;
        } else if (m(1,1) > m(2,2)) {
            float s = 2.0f * std::sqrt(1.0f + m(1,1) - m(0,0) - m(2,2));
            w = (m(0,2) - m(2,0)) / s;
            x = (m(0,1) + m(1,0)) / s;
            y = 0.25f * s;
            z = (m(1,2) + m(2,1)) / s;
        } else {
            float s = 2.0f * std::sqrt(1.0f + m(2,2) - m(0,0) - m(1,1));
            w = (m(1,0) - m(0,1)) / s;
            x = (m(0,2) + m(2,0)) / s;
            y = (m(1,2) + m(2,1)) / s;
            z = 0.25f * s;
        }
    }
}

// Assignment
Quat& Quat::operator+=(const Quat& other) {
    x += other.x; y += other.y; z += other.z; w += other.w; return *this;
}
Quat& Quat::operator-=(const Quat& other) {
    x -= other.x; y -= other.y; z -= other.z; w -= other.w; return *this;
}
Quat& Quat::operator*=(const Quat& other) { *this = *this * other; return *this; }
Quat& Quat::operator*=(float scalar) {
    x *= scalar; y *= scalar; z *= scalar; w *= scalar; return *this;
}
Quat& Quat::operator/=(float scalar) {
    float inv = 1.0f / scalar; x *= inv; y *= inv; z *= inv; w *= inv; return *this;
}

// Arithmetic
Quat Quat::operator+(const Quat& other) const {
    return Quat(x+other.x, y+other.y, z+other.z, w+other.w);
}
Quat Quat::operator-(const Quat& other) const {
    return Quat(x-other.x, y-other.y, z-other.z, w-other.w);
}
Quat Quat::operator-() const { return Quat(-x, -y, -z, -w); }
Quat Quat::operator*(const Quat& other) const {
    return Quat(
        w*other.x + x*other.w + y*other.z - z*other.y,
        w*other.y - x*other.z + y*other.w + z*other.x,
        w*other.z + x*other.y - y*other.x + z*other.w,
        w*other.w - x*other.x - y*other.y - z*other.z
    );
}
Quat Quat::operator*(float scalar) const {
    return Quat(x*scalar, y*scalar, z*scalar, w*scalar);
}
Quat Quat::operator/(float scalar) const {
    float inv = 1.0f / scalar; return Quat(x*inv, y*inv, z*inv, w*inv);
}

Vec3 Quat::operator*(const Vec3& v) const {
    Vec3 qvec(x, y, z);
    Vec3 cross1 = cross(qvec, v);
    Vec3 cross2 = cross(qvec, cross1);
    return v + cross1 * (2.0f * w) + cross2 * 2.0f;
}

// Comparison
bool Quat::operator==(const Quat& other) const {
    return x == other.x && y == other.y && z == other.z && w == other.w;
}
bool Quat::operator!=(const Quat& other) const { return !(*this == other); }

// Quaternion operations
Quat Quat::normalized() const {
    float len = length(); return len > 0.0f ? *this / len : *this;
}
void Quat::normalize() { *this = normalized(); }

Quat Quat::conjugate() const { return Quat(-x, -y, -z, w); }

Quat Quat::inverse() const {
    float lenSq = lengthSquared();
    return lenSq > 0.0f ? conjugate() / lenSq : *this;
}

float Quat::length() const { return std::sqrt(x*x + y*y + z*z + w*w); }
float Quat::lengthSquared() const { return x*x + y*y + z*z + w*w; }
float Quat::dot(const Quat& other) const {
    return x*other.x + y*other.y + z*other.z + w*other.w;
}

Vec3 Quat::eulerAngles() const {
    Quat q = normalized();
    float sinr_cosp = 2.0f * (q.w * q.x + q.y * q.z);
    float cosr_cosp = 1.0f - 2.0f * (q.x * q.x + q.y * q.y);
    float roll = std::atan2(sinr_cosp, cosr_cosp);

    float sinp = 2.0f * (q.w * q.y - q.z * q.x);
    float pitch = (std::abs(sinp) >= 1.0f) ?
        std::copysign(3.14159265359f / 2.0f, sinp) : std::asin(sinp);

    float siny_cosp = 2.0f * (q.w * q.z + q.x * q.y);
    float cosy_cosp = 1.0f - 2.0f * (q.y * q.y + q.z * q.z);
    float yaw = std::atan2(siny_cosp, cosy_cosp);

    return Vec3(roll, pitch, yaw);
}

Mat4 Quat::toMatrix() const {
    Quat q = normalized();
    float xx = q.x*q.x, yy = q.y*q.y, zz = q.z*q.z;
    float xy = q.x*q.y, xz = q.x*q.z, yz = q.y*q.z;
    float wx = q.w*q.x, wy = q.w*q.y, wz = q.w*q.z;

    Mat4 m;
    m(0,0) = 1.0f - 2.0f*(yy + zz); m(0,1) = 2.0f*(xy - wz); m(0,2) = 2.0f*(xz + wy);
    m(1,0) = 2.0f*(xy + wz); m(1,1) = 1.0f - 2.0f*(xx + zz); m(1,2) = 2.0f*(yz - wx);
    m(2,0) = 2.0f*(xz - wy); m(2,1) = 2.0f*(yz + wx); m(2,2) = 1.0f - 2.0f*(xx + yy);
    m(3,3) = 1.0f;
    return m;
}

// Static
Quat Quat::identity() { return Quat(0.0f, 0.0f, 0.0f, 1.0f); }

Quat Quat::fromEulerAngles(const Vec3& euler) {
    float roll = euler.x, pitch = euler.y, yaw = euler.z;
    float cy = std::cos(yaw * 0.5f), sy = std::sin(yaw * 0.5f);
    float cp = std::cos(pitch * 0.5f), sp = std::sin(pitch * 0.5f);
    float cr = std::cos(roll * 0.5f), sr = std::sin(roll * 0.5f);

    return Quat(
        sr * cp * cy - cr * sp * sy,
        cr * sp * cy + sr * cp * sy,
        cr * cp * sy - sr * sp * cy,
        cr * cp * cy + sr * sp * sy
    );
}

Quat Quat::lookAt(const Vec3& direction, const Vec3& up) {
    Vec3 forward = normalize(direction);
    Vec3 right = normalize(cross(up, forward));
    Vec3 up2 = cross(forward, right);

    Mat4 m = Mat4::identity();
    m(0,0) = right.x; m(0,1) = right.y; m(0,2) = right.z;
    m(1,0) = up2.x; m(1,1) = up2.y; m(1,2) = up2.z;
    m(2,0) = -forward.x; m(2,1) = -forward.y; m(2,2) = -forward.z;

    return Quat(m);
}

Quat Quat::slerp(const Quat& a, const Quat& b, float t) {
    float dot = a.dot(b);
    Quat qb = b;
    if (dot < 0.0f) { qb = -b; dot = -dot; }

    const float DOT_THRESHOLD = 0.9995f;
    if (dot > DOT_THRESHOLD) {
        Quat result = a + (qb - a) * t; result.normalize(); return result;
    }

    float theta_0 = std::acos(dot);
    float theta = theta_0 * t;
    float sin_theta = std::sin(theta);
    float sin_theta_0 = std::sin(theta_0);
    float s0 = std::cos(theta) - dot * sin_theta / sin_theta_0;
    float s1 = sin_theta / sin_theta_0;

    return (a * s0) + (qb * s1);
}

Quat Quat::lerp(const Quat& a, const Quat& b, float t) {
    Quat result = a + (b - a) * t; result.normalize(); return result;
}

// Output
std::ostream& operator<<(std::ostream& os, const Quat& q) {
    os << "Quat(" << q.x << ", " << q.y << ", " << q.z << ", " << q.w << ")"; return os;
}

// Non-member
Quat operator*(float scalar, const Quat& q) { return q * scalar; }
float dot(const Quat& a, const Quat& b) { return a.dot(b); }
Quat normalize(const Quat& q) { return q.normalized(); }
Quat conjugate(const Quat& q) { return q.conjugate(); }
Quat inverse(const Quat& q) { return q.inverse(); }
Quat slerp(const Quat& a, const Quat& b, float t) { return Quat::slerp(a, b, t); }
Quat lerp(const Quat& a, const Quat& b, float t) { return Quat::lerp(a, b, t); }

} // namespace ygl
