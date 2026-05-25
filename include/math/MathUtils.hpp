#pragma once
#include <cmath>
#include <algorithm>
#include "Vec3.hpp"
#include "Vec4.hpp"
#include "Mat4.hpp"

namespace ygl {

// Fonctions utilitaires manquantes
template<typename T>
T lerp(const T& a, const T& b, float t) {
    return a + (b - a) * t;
}

template<typename T>
T clamp(const T& v, const T& min, const T& max) {
    return std::max(min, std::min(max, v));
}

inline bool isZero(float v, float epsilon = 1e-6f) {
    return std::abs(v) < epsilon;
}

inline bool Vec3::isZero(float epsilon = 1e-6f) const {
    return lengthSquared() < epsilon * epsilon;
}

} // namespace ygl
