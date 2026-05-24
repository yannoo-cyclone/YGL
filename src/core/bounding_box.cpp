#include "core/bounding_box.hpp"
#include "math/vec3.hpp"
#include "math/ray.hpp"
#include "math/mat4.hpp"
#include <vector>
#include <algorithm>
#include <ostream>

namespace ygl {

// ============================================
// Constructeurs / Destructeur
// ============================================
BoundingBox::BoundingBox() {
    Reset();
}

BoundingBox::BoundingBox(const Vec3& min, const Vec3& max)
    : min(min), max(max) {}

BoundingBox::BoundingBox(const BoundingBox& other)
    : min(other.min), max(other.max) {}

BoundingBox& BoundingBox::operator=(const BoundingBox& other) {
    if (this != &other) {
        min = other.min;
        max = other.max;
    }
    return *this;
}

// ============================================
// Méthodes de base
// ============================================
void BoundingBox::Reset() {
    min = Vec3(std::numeric_limits<float>::max());
    max = Vec3(std::numeric_limits<float>::lowest());
}

const Vec3& BoundingBox::GetMin() const {
    return min;
}

const Vec3& BoundingBox::GetMax() const {
    return max;
}

Vec3 BoundingBox::GetCenter() const {
    return (min + max) * 0.5f;
}

Vec3 BoundingBox::GetSize() const {
    return max - min;
}

Vec3 BoundingBox::GetHalfSize() const {
    return GetSize() * 0.5f;
}

float BoundingBox::GetWidth() const {
    return max.x - min.x;
}

float BoundingBox::GetHeight() const {
    return max.y - min.y;
}

float BoundingBox::GetDepth() const {
    return max.z - min.z;
}

float BoundingBox::GetVolume() const {
    Vec3 size = GetSize();
    return size.x * size.y * size.z;
}

float BoundingBox::GetSurfaceArea() const {
    Vec3 size = GetSize();
    return 2.0f * (size.x * size.y + size.x * size.z + size.y * size.z);
}

// ============================================
// Setters
// ============================================
void BoundingBox::SetMin(const Vec3& min_val) {
    min = min_val;
}

void BoundingBox::SetMax(const Vec3& max_val) {
    max = max_val;
}

void BoundingBox::Set(const Vec3& min_val, const Vec3& max_val) {
    min = min_val;
    max = max_val;
}

void BoundingBox::SetCenter(const Vec3& center) {
    Vec3 size = GetSize();
    min = center - size * 0.5f;
    max = center + size * 0.5f;
}

void BoundingBox::SetSize(const Vec3& size) {
    Vec3 center = GetCenter();
    min = center - size * 0.5f;
    max = center + size * 0.5f;
}

// ============================================
// Expansion
// ============================================
void BoundingBox::Expand(const Vec3& point) {
    min = Vec3::Min(min, point);
    max = Vec3::Max(max, point);
}

void BoundingBox::Expand(const BoundingBox& other) {
    Expand(other.min);
    Expand(other.max);
}

// ============================================
// Tests de collision/distance
// ============================================
bool BoundingBox::Contains(const Vec3& point) const {
    return point.x >= min.x && point.x <= max.x &&
           point.y >= min.y && point.y <= max.y &&
           point.z >= min.z && point.z <= max.z;
}

bool BoundingBox::Contains(const BoundingBox& other) const {
    return Contains(other.min) && Contains(other.max);
}

bool BoundingBox::Intersects(const BoundingBox& other) const {
    return !(other.max.x < min.x || other.min.x > max.x ||
             other.max.y < min.y || other.min.y > max.y ||
             other.max.z < min.z || other.min.z > max.z);
}

bool BoundingBox::Intersect(const Ray& ray, float t_min, float t_max) const {
    // Implémentation simplifiée (à adapter selon votre classe Ray)
    float t1 = (min.x - ray.origin.x) / ray.direction.x;
    float t2 = (max.x - ray.origin.x) / ray.direction.x;
    float t3 = (min.y - ray.origin.y) / ray.direction.y;
    float t4 = (max.y - ray.origin.y) / ray.direction.y;
    float t5 = (min.z - ray.origin.z) / ray.direction.z;
    float t6 = (max.z - ray.origin.z) / ray.direction.z;

    float tmin = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
    float tmax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));

    return tmax >= std::max(tmin, t_min) && tmin <= std::min(tmax, t_max);
}

// Si HitInfo est défini, décommentez et implémentez :
/*
bool BoundingBox::Intersect(const Ray& ray, HitInfo& hit_info, float t_min, float t_max) const {
    if (Intersect(ray, t_min, t_max)) {
        // Remplir hit_info ici
        return true;
    }
    return false;
}
*/

float BoundingBox::DistanceTo(const Vec3& point) const {
    Vec3 closest = Vec3::Clamp(point, min, max);
    return (point - closest).Length();
}

float BoundingBox::DistanceTo(const BoundingBox& other) const {
    if (Intersects(other)) return 0.0f;
    return std::min(DistanceTo(other.min), DistanceTo(other.max));
}

// ============================================
// Corners
// ============================================
void BoundingBox::GetCorners(Vec3 corners[8]) const {
    corners[0] = Vec3(min.x, min.y, min.z);
    corners[1] = Vec3(max.x, min.y, min.z);
    corners[2] = Vec3(max.x, max.y, min.z);
    corners[3] = Vec3(min.x, max.y, min.z);
    corners[4] = Vec3(min.x, min.y, max.z);
    corners[5] = Vec3(max.x, min.y, max.z);
    corners[6] = Vec3(max.x, max.y, max.z);
    corners[7] = Vec3(min.x, max.y, max.z);
}

std::vector<Vec3> BoundingBox::GetCorners() const {
    Vec3 temp_corners[8];
    GetCorners(temp_corners);
    return std::vector<Vec3>(temp_corners, temp_corners + 8);
}

// ============================================
// Transformations
// ============================================
BoundingBox BoundingBox::Transform(const mat4& matrix) const {
    Vec3 corners[8];
    GetCorners(corners);
    Vec3 new_min = corners[0].Transform(matrix);
    Vec3 new_max = new_min;
    for (int i = 1; i < 8; ++i) {
        Vec3 corner = corners[i].Transform(matrix);
        new_min = Vec3::Min(new_min, corner);
        new_max = Vec3::Max(new_max, corner);
    }
    return BoundingBox(new_min, new_max);
}

// ============================================
// Méthodes statiques
// ============================================
BoundingBox BoundingBox::FromPoints(const std::vector<Vec3>& points) {
    BoundingBox box;
    for (const Vec3& point : points) {
        box.Expand(point);
    }
    return box;
}

BoundingBox BoundingBox::FromCenterAndSize(const Vec3& center, const Vec3& size) {
    return BoundingBox(center - size * 0.5f, center + size * 0.5f);
}

BoundingBox BoundingBox::FromCenterAndHalfSize(const Vec3& center, const Vec3& half_size) {
    return BoundingBox(center - half_size, center + half_size);
}

BoundingBox BoundingBox::Merge(const BoundingBox& a, const BoundingBox& b) {
    return BoundingBox(
        Vec3::Min(a.min, b.min),
        Vec3::Max(a.max, b.max)
    );
}

// ============================================
// Opérateurs
// ============================================
bool BoundingBox::operator==(const BoundingBox& other) const {
    return min == other.min && max == other.max;
}

bool BoundingBox::operator!=(const BoundingBox& other) const {
    return !(*this == other);
}

// ============================================
// Affichage
// ============================================
std::ostream& operator<<(std::ostream& os, const BoundingBox& box) {
    os << "BoundingBox(min=" << box.min << ", max=" << box.max << ")";
    return os;
}

} // namespace ygl
