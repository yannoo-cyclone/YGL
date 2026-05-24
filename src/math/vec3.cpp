#include "math/vec3.hpp"
#include "math/mat4.hpp"

namespace ygl {

// Seule méthode non-inline dans le .hpp
Vec3 Vec3::Transform(const mat4& matrix) const {
    float w = matrix(3, 0) * x + matrix(3, 1) * y + matrix(3, 2) * z + matrix(3, 3);
    return Vec3(
        (matrix(0, 0) * x + matrix(0, 1) * y + matrix(0, 2) * z + matrix(0, 3)) / w,
        (matrix(1, 0) * x + matrix(1, 1) * y + matrix(1, 2) * z + matrix(1, 3)) / w,
        (matrix(2, 0) * x + matrix(2, 1) * y + matrix(2, 2) * z + matrix(2, 3)) / w
    );
}

} // namespace ygl
