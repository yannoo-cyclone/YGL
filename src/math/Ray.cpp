#include "math/Ray.hpp"
#include "math/Mat4.hpp"
namespace ygl {
Ray::Ray() : origin(Vec3()), direction(Vec3()), t_min(0.0f), t_max(std::numeric_limits<float>::max()) {}
Ray::Ray(const Vec3& origin, const Vec3& direction) : origin(origin), direction(direction), t_min(0.0f), t_max(std::numeric_limits<float>::max()) {}
Vec3 Ray::operator()(float t) const { return origin + direction * t; }
Vec3 Ray::at(float t) const { return origin + direction * t; }
const Vec3& Ray::getOrigin() const { return origin; }
const Vec3& Ray::getDirection() const { return direction; }
float Ray::getTMin() const { return t_min; }
float Ray::getTMax() const { return t_max; }
void Ray::setOrigin(const Vec3& o) { origin = o; }
void Ray::setDirection(const Vec3& d) { direction = d; }
void Ray::setTMin(float t) { t_min = t; }
void Ray::setTMax(float t) { t_max = t; }
void Ray::setBounds(float min, float max) { t_min = min; t_max = max; }
bool Ray::isValid() const { return direction.lengthSquared() > 0.0f; }
Ray Ray::transform(const Mat4& matrix) const { return Ray(matrix * origin, matrix * (origin + direction) - matrix * origin); }
std::ostream& operator<<(std::ostream& os, const Ray& ray) { os << "Ray(origin=" << ray.origin << ", direction=" << ray.direction << ", t_min=" << ray.t_min << ", t_max=" << ray.t_max << ")"; return os; }
} // namespace ygl
