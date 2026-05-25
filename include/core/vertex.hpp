#pragma once
#include "math/Vec3.hpp"
#include "math/Vec2.hpp"

namespace ygl {
struct Vertex {
    Vec3 position;
    Vec2 texcoord;
    Vec3 normal;
};
} // namespace ygl
