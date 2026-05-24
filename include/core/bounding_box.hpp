#pragma once

#include "math/vec3.hpp"
#include "math/ray.hpp"
#include <vector>

namespace ygl {

class Ray;
class HitInfo;  // Forward declaration (ou inclure le header si HitInfo est défini ailleurs)

class BoundingBox {
public:
    Vec3 min;
    Vec3 max;

    BoundingBox();
    BoundingBox(const Vec3& min, const Vec3& max);
    BoundingBox(const BoundingBox& other);
    ~BoundingBox() = default;

    BoundingBox& operator=(const BoundingBox& other);

    void Reset();
    const Vec3& GetMin() const;
    const Vec3& GetMax() const;
    Vec3 GetCenter() const;
    Vec3 GetSize() const;
    Vec3 GetHalfSize() const;
    float GetWidth() const;
    float GetHeight() const;
    float GetDepth() const;
    float GetVolume() const;
    float GetSurfaceArea() const;

    void SetMin(const Vec3& min_val);
    void SetMax(const Vec3& max_val);
    void Set(const Vec3& min_val, const Vec3& max_val);
    void SetCenter(const Vec3& center);
    void SetSize(const Vec3& size);

    void Expand(const Vec3& point);
    void Expand(const BoundingBox& other);

    bool Contains(const Vec3& point) const;
    bool Contains(const BoundingBox& other) const;
    bool Intersects(const BoundingBox& other) const;
    bool Intersect(const Ray& ray, float t_min, float t_max) const;
    bool Intersect(const Ray& ray, HitInfo& hit_info, float t_min, float t_max) const;

    float DistanceTo(const Vec3& point) const;
    float DistanceTo(const BoundingBox& other) const;

    void GetCorners(Vec3 corners[8]) const;
    std::vector<Vec3> GetCorners() const;

    BoundingBox Transform(const class mat4& matrix) const;

    static BoundingBox FromPoints(const std::vector<Vec3>& points);
    static BoundingBox FromCenterAndSize(const Vec3& center, const Vec3& size);
    static BoundingBox FromCenterAndHalfSize(const Vec3& center, const Vec3& half_size);
    static BoundingBox Merge(const BoundingBox& a, const BoundingBox& b);

    bool operator==(const BoundingBox& other) const;
    bool operator!=(const BoundingBox& other) const;
};

std::ostream& operator<<(std::ostream& os, const BoundingBox& box);

} // namespace ygl
