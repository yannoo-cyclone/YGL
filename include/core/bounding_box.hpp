#pragma once

#include "math/vec3.hpp"
#include "math/ray.hpp"
#include <limits>

namespace ygl {

class BoundingBox {
public:
    Vec3 min;
    Vec3 max;

    BoundingBox();
    BoundingBox(const Vec3& min, const Vec3& max);

    void reset();
    void expand(const Vec3& point);
    void expand(const BoundingBox& other);

    bool isValid() const;
    Vec3 getCenter() const;
    Vec3 getSize() const;
    Vec3 getHalfSize() const;

    bool contains(const Vec3& point) const;
    bool contains(const BoundingBox& other) const;
    bool intersects(const BoundingBox& other) const;
    bool intersect(const Ray& ray, float& tmin, float& tmax) const;

    float getSurfaceArea() const;
    float getVolume() const;

    static BoundingBox empty();
    static BoundingBox infinite();
};

} // namespace ygl

