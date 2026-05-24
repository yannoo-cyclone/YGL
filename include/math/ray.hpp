#pragma once

#include "vec3.hpp"
#include <limits>
#include <iostream>

namespace ygl {

class mat4;

class Ray {
public:
    Vec3 origin;
    Vec3 direction;
    float t_min;
    float t_max;

    Ray();
    Ray(const Vec3& origin, const Vec3& direction);

    Vec3 operator()(float t) const;
    Vec3 at(float t) const;

    const Vec3& getOrigin() const;
    const Vec3& getDirection() const;
    float getTMin() const;
    float getTMax() const;

    void setOrigin(const Vec3& o);
    void setDirection(const Vec3& d);
    void setTMin(float t);
    void setTMax(float t);
    void setBounds(float min, float max);

    bool isValid() const;
    Ray transform(const mat4& matrix) const;
};

class DifferentialRay : public Ray {
public:
    Vec3 rx_origin;
    Vec3 ry_origin;
    Vec3 rx_direction;
    Vec3 ry_direction;
    bool has_differentials;

    DifferentialRay();
    DifferentialRay(const Vec3& origin, const Vec3& direction, float t_min = 0.0f, float t_max = std::numeric_limits<float>::max());
    DifferentialRay(
        const Vec3& origin, const Vec3& direction,
        const Vec3& rx_origin, const Vec3& ry_origin,
        const Vec3& rx_direction, const Vec3& ry_direction,
        float t_min = 0.0f, float t_max = std::numeric_limits<float>::max()
    );

    Vec3 getRxOrigin() const;
    Vec3 getRyOrigin() const;
    Vec3 getRxDirection() const;
    Vec3 getRyDirection() const;
    bool getHasDifferentials() const;

    void setHasDifferentials(bool has);
    void setRxOrigin(const Vec3& o);
    void setRyOrigin(const Vec3& o);
    void setRxDirection(const Vec3& d);
    void setRyDirection(const Vec3& d);
    void scaleDifferentials(float s);
    DifferentialRay transform(const mat4& matrix) const;
};

std::ostream& operator<<(std::ostream& os, const Ray& ray);
std::ostream& operator<<(std::ostream& os, const DifferentialRay& ray);

} // namespace ygl
