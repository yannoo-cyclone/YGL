#pragma once

#include "vec3.hpp"

namespace ygl {

class Ray {
public:
    Vec3 origin;
    Vec3 direction;
    Vec3 invDirection;
    int sign[3];

    Ray();
    Ray(const Vec3& origin, const Vec3& direction);

    void updateInverse();
    Vec3 at(float t) const;
    bool isNormalized() const;
};

} // namespace ygl

