#include "math/vec2.hpp"

namespace ygl {

// Opérateur non-member
Vec2 operator*(float scalar, const Vec2& v) {
    return v * scalar;
}

} // namespace ygl
