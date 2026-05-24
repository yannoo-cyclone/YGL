#pragma once
#include "math/vec3.hpp"
#include "math/vec2.hpp"

namespace ygl {
struct Vertex {
    Vec3 position;
    Vec2 texcoord;
    Vec3 normal;
};
} // namespace ygl
