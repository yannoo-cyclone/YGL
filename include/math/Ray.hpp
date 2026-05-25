#pragma once
#include "Vec3.hpp"
#include <limits>
#include <iostream>

namespace ygl {
class Mat4;
class Ray {
public:
    Vec3 origin; Vec3 direction; float t_min; float t_max;
    Ray(); Ray(const Vec3& origin, const Vec3& direction);
    Vec3 operator()(float t) const; Vec3 at(float t) const;
    const Vec3& getOrigin() const; const Vec3& getDirection() const;
    float getTMin() const; float getTMax() const;
    void setOrigin(const Vec3& o); void setDirection(const Vec3& d);
    void setTMin(float t); void setTMax(float t); void setBounds(float min, float max);
    bool isValid() const; Ray transform(const Mat4& matrix) const;
};
std::ostream& operator<<(std::ostream& os, const Ray& ray);
} // namespace ygl
