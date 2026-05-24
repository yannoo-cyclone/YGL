#ifndef YGL_MATH_UTILS_HPP
#define YGL_MATH_UTILS_HPP

#include <cmath>
#include <limits>
#include <algorithm>
#include <cstdint> // Pour uint32_t

namespace ygl {

// === CONSTANTES ===
constexpr float EPSILON          = 1e-6f;
constexpr float PI               = 3.14159265358979323846f;
constexpr float TWO_PI           = 2.0f * PI;
constexpr float HALF_PI          = PI / 2.0f;
constexpr float INV_PI           = 1.0f / PI;
constexpr float INV_TWO_PI       = 1.0f / TWO_PI;
constexpr float DEG_TO_RAD       = PI / 180.0f;
constexpr float RAD_TO_DEG       = 180.0f / PI;

// === COMPARAISONS FLOTTANTES ===
inline bool AlmostEqual(float a, float b, float epsilon = EPSILON) {
    return std::abs(a - b) <= epsilon;
}
inline bool AlmostZero(float a, float epsilon = EPSILON) {
    return std::abs(a) <= epsilon;
}

// === INTERPOLATION & CLAMPING ===
template<typename T>
inline T Clamp(T value, T min, T max) { return std::clamp(value, min, max); }

template<typename T>
inline T Lerp(T a, T b, float t) { return a + (b - a) * t; }

inline float Smoothstep(float edge0, float edge1, float x) {
    float t = Clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
    return t * t * (3.0f - 2.0f * t);
}

// === CONVERSIONS ANGULAIRES ===
inline float ToRadians(float degrees) { return degrees * DEG_TO_RAD; }
inline float ToDegrees(float radians) { return radians * RAD_TO_DEG; }

// === SÉCURITÉ ===
inline float SafeDivide(float numerator, float denominator, float default_value = 0.0f) {
    return (std::abs(denominator) > EPSILON) ? (numerator / denominator) : default_value;
}
inline bool IsFinite(float x) { return std::isfinite(x); }
inline bool IsNaN(float x) { return std::isnan(x); }
inline bool IsInfinity(float x) { return std::isinf(x); }

// === MODULO & WRAPPING ===
inline float Mod(float a, float b) {
    float result = std::fmod(a, b);
    return (result < 0.0f) ? result + b : result;
}
inline float Wrap(float value, float min, float max) {
    float range = max - min;
    return min + Mod(value - min, range);
}
inline float PingPong(float value, float length) {
    float mod = Mod(value, length * 2.0f);
    return (mod > length) ? length * 2.0f - mod : mod;
}

// === RACINE CARRÉE RAPIDE (Quake III) ===
inline float FastInvSqrt(float x) {
    float x2 = x * 0.5f;
    float y = x;
    int32_t i = *reinterpret_cast<int32_t*>(&y);
    i = 0x5F3759DF - (i >> 1);
    y = *reinterpret_cast<float*>(&i);
    y = y * (1.5f - (x2 * y * y));
    return y;
}
inline float FastSqrt(float x) { return x * FastInvSqrt(x); }

// === ALÉATOIRE ===
inline float RandomFloat() {
    return static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
}
inline float RandomFloat(float min, float max) {
    return min + (max - min) * RandomFloat();
}

// === HACHAGE 3D (pour BVH/spatial grid) ===
inline uint32_t HashCoordinate(uint32_t x, uint32_t y, uint32_t z) {
    uint32_t hash = x;
    hash = hash * 31 + y;
    hash = hash * 31 + z;
    return hash;
}

// === CODE DE MORTON (Z-order curve) ===
inline uint32_t MortonCode(uint32_t x, uint32_t y, uint32_t z) {
    uint32_t result = 0;
    for (uint32_t i = 0; i < 10; ++i) {
        result |= ((x & (1 << i)) << (2 * i));
        result |= ((y & (1 << i)) << (2 * i + 1));
        result |= ((z & (1 << i)) << (2 * i + 2));
    }
    return result;
}

// === PUISSANCES DE 2 ===
inline uint32_t NextPowerOfTwo(uint32_t x) {
    x--; x |= x >> 1; x |= x >> 2; x |= x >> 4;
    x |= x >> 8; x |= x >> 16; return x + 1;
}
inline bool IsPowerOfTwo(uint32_t x) { return (x != 0) && ((x & (x - 1)) == 0); }
inline uint32_t Log2(uint32_t x) { uint32_t r = 0; while (x >>= 1) ++r; return r; }

} // namespace ygl

#endif // YGL_MATH_UTILS_HPP
