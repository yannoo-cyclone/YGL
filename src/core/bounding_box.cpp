#include "core/bounding_box.hpp"
#include "math/vec3.hpp"
#include "math/ray.hpp"
#include "math/math_utils.hpp"
#include <algorithm>
#include <limits>

namespace ygl {

// --- Constructors ---
BoundingBox::BoundingBox() {
    Reset();
}

BoundingBox::BoundingBox(const vec3& min, const vec3& max)
    : min_bound(min), max_bound(max) {
}

BoundingBox::BoundingBox(const BoundingBox& other)
    : min_bound(other.min_bound), max_bound(other.max_bound) {
}

// --- Assignment ---
BoundingBox& BoundingBox::operator=(const BoundingBox& other) {
    min_bound = other.min_bound;
    max_bound = other.max_bound;
    return *this;
}

// --- Reset ---
void BoundingBox::Reset() {
    min_bound = vec3(std::numeric_limits<float>::max());
    max_bound = vec3(std::numeric_limits<float>::lowest());
}

// --- Accessors ---
const vec3& BoundingBox::GetMin() const {
    return min_bound;
}

const vec3& BoundingBox::GetMax() const {
    return max_bound;
}

vec3 BoundingBox::GetCenter() const {
    return (min_bound + max_bound) * 0.5f;
}

vec3 BoundingBox::GetSize() const {
    return max_bound - min_bound;
}

vec3 BoundingBox::GetHalfSize() const {
    return GetSize() * 0.5f;
}

float BoundingBox::GetWidth() const {
    return max_bound.x - min_bound.x;
}

float BoundingBox::GetHeight() const {
    return max_bound.y - min_bound.y;
}

float BoundingBox::GetDepth() const {
    return max_bound.z - min_bound.z;
}

float BoundingBox::GetVolume() const {
    vec3 size = GetSize();
    return size.x * size.y * size.z;
}

float BoundingBox::GetSurfaceArea() const {
    vec3 size = GetSize();
    return 2.0f * (size.x * size.y + size.x * size.z + size.y * size.z);
}

// --- Setters ---
void BoundingBox::SetMin(const vec3& min_val) {
    min_bound = min_val;
}

void BoundingBox::SetMax(const vec3& max_val) {
    max_bound = max_val;
}

void BoundingBox::Set(const vec3& min_val, const vec3& max_val) {
    min_bound = min_val;
    max_bound = max_val;
}

void BoundingBox::SetCenter(const vec3& center) {
    vec3 size = GetSize();
    min_bound = center - size * 0.5f;
    max_bound = center + size * 0.5f;
}

void BoundingBox::SetSize(const vec3& size) {
    vec3 center = GetCenter();
    min_bound = center - size * 0.5f;
    max_bound = center + size * 0.5f;
}

// --- Expand ---
void BoundingBox::Expand(const vec3& point) {
    min_bound = vec3(
        std::min(min_bound.x, point.x),
        std::min(min_bound.y, point.y),
        std::min(min_bound.z, point.z)
    );
    max_bound = vec3(
        std::max(max_bound.x, point.x),
        std::max(max_bound.y, point.y),
        std::max(max_bound.z, point.z)
    );
}

void BoundingBox::Expand(const BoundingBox& other) {
    min_bound = vec3(
        std::min(min_bound.x, other.min_bound.x),
        std::min(min_bound.y, other.min_bound.y),
        std::min(min_bound.z, other.min_bound.z)
    );
    max_bound = vec3(
        std::max(max_bound.x, other.max_bound.x),
        std::max(max_bound.y, other.max_bound.y),
        std::max(max_bound.z, other.max_bound.z)
    );
}

// --- Contains ---
bool BoundingBox::Contains(const vec3& point) const {
    return point.x >= min_bound.x && point.x <= max_bound.x &&
           point.y >= min_bound.y && point.y <= max_bound.y &&
           point.z >= min_bound.z && point.z <= max_bound.z;
}

bool BoundingBox::Contains(const BoundingBox& other) const {
    return min_bound.x <= other.min_bound.x && max_bound.x >= other.max_bound.x &&
           min_bound.y <= other.min_bound.y && max_bound.y >= other.max_bound.y &&
           min_bound.z <= other.min_bound.z && max_bound.z >= other.max_bound.z;
}

// --- Intersects ---
bool BoundingBox::Intersects(const BoundingBox& other) const {
    return !(max_bound.x < other.min_bound.x || min_bound.x > other.max_bound.x ||
             max_bound.y < other.min_bound.y || min_bound.y > other.max_bound.y ||
             max_bound.z < other.min_bound.z || min_bound.z > other.max_bound.z);
}

// --- Ray Intersection (Slab Method) ---
bool BoundingBox::Intersect(const Ray& ray, float t_min, float t_max) const {
    float t_enter = t_min;
    float t_exit = t_max;

    vec3 inv_dir = vec3(
        (ray.direction.x != 0.0f) ? 1.0f / ray.direction.x : std::numeric_limits<float>::infinity(),
        (ray.direction.y != 0.0f) ? 1.0f / ray.direction.y : std::numeric_limits<float>::infinity(),
        (ray.direction.z != 0.0f) ? 1.0f / ray.direction.z : std::numeric_limits<float>::infinity()
    );

    vec3 t_min_vec = (min_bound - ray.origin) * inv_dir;
    vec3 t_max_vec = (max_bound - ray.origin) * inv_dir;

    vec3 t_min_sorted = vec3(
        std::min(t_min_vec.x, t_max_vec.x),
        std::min(t_min_vec.y, t_max_vec.y),
        std::min(t_min_vec.z, t_max_vec.z)
    );

    vec3 t_max_sorted = vec3(
        std::max(t_min_vec.x, t_max_vec.x),
        std::max(t_min_vec.y, t_max_vec.y),
        std::max(t_min_vec.z, t_max_vec.z)
    );

    t_enter = std::max(t_min_sorted.x, std::max(t_min_sorted.y, t_min_sorted.z));
    t_exit = std::min(t_max_sorted.x, std::min(t_max_sorted.y, t_max_sorted.z));

    return t_enter <= t_exit && t_exit >= t_min;
}

bool BoundingBox::Intersect(const Ray& ray, HitInfo& hit_info, float t_min, float t_max) const {
    if (!Intersect(ray, t_min, t_max)) {
        return false;
    }

    float t_enter = t_min;
    float t_exit = t_max;

    vec3 inv_dir = vec3(
        (ray.direction.x != 0.0f) ? 1.0f / ray.direction.x : std::numeric_limits<float>::infinity(),
        (ray.direction.y != 0.0f) ? 1.0f / ray.direction.y : std::numeric_limits<float>::infinity(),
        (ray.direction.z != 0.0f) ? 1.0f / ray.direction.z : std::numeric_limits<float>::infinity()
    );

    vec3 t_min_vec = (min_bound - ray.origin) * inv_dir;
    vec3 t_max_vec = (max_bound - ray.origin) * inv_dir;

    t_enter = std::max(
        std::max(std::min(t_min_vec.x, t_max_vec.x), std::min(t_min_vec.y, t_max_vec.y)),
        std::min(t_min_vec.z, t_max_vec.z)
    );

    t_exit = std::min(
        std::min(std::max(t_min_vec.x, t_max_vec.x), std::max(t_min_vec.y, t_max_vec.y)),
        std::max(t_min_vec.z, t_max_vec.z)
    );

    if (t_enter > t_exit) {
        std::swap(t_enter, t_exit);
    }

    if (t_enter < t_min) {
        t_enter = t_min;
    }

    if (t_exit > t_max) {
        t_exit = t_max;
    }

    if (t_enter >= t_exit) {
        return false;
    }

    hit_info.t = t_enter;
    hit_info.position = ray(t_enter);
    hit_info.normal = vec3(0.0f, 0.0f, 0.0f);
    hit_info.object = nullptr;
    hit_info.material = nullptr;

    vec3 hit_point = ray(t_enter);
    vec3 rel_point = hit_point - min_bound;

    if (AlmostEqual(rel_point.x, 0.0f, EPSILON)) hit_info.normal = vec3(-1.0f, 0.0f, 0.0f);
    else if (AlmostEqual(rel_point.x, GetWidth(), EPSILON)) hit_info.normal = vec3(1.0f, 0.0f, 0.0f);
    else if (AlmostEqual(rel_point.y, 0.0f, EPSILON)) hit_info.normal = vec3(0.0f, -1.0f, 0.0f);
    else if (AlmostEqual(rel_point.y, GetHeight(), EPSILON)) hit_info.normal = vec3(0.0f, 1.0f, 0.0f);
    else if (AlmostEqual(rel_point.z, 0.0f, EPSILON)) hit_info.normal = vec3(0.0f, 0.0f, -1.0f);
    else if (AlmostEqual(rel_point.z, GetDepth(), EPSILON)) hit_info.normal = vec3(0.0f, 0.0f, 1.0f);

    return true;
}

// --- Distance ---
float BoundingBox::DistanceTo(const vec3& point) const {
    float dx = std::max(std::max(min_bound.x - point.x, 0.0f), point.x - max_bound.x);
    float dy = std::max(std::max(min_bound.y - point.y, 0.0f), point.y - max_bound.y);
    float dz = std::max(std::max(min_bound.z - point.z, 0.0f), point.z - max_bound.z);
    return std::sqrt(dx * dx + dy * dy + dz * dz);
}

float BoundingBox::DistanceTo(const BoundingBox& other) const {
    float dx = std::max(std::max(min_bound.x - other.max_bound.x, 0.0f), other.min_bound.x - max_bound.x);
    float dy = std::max(std::max(min_bound.y - other.max_bound.y, 0.0f), other.min_bound.y - max_bound.y);
    float dz = std::max(std::max(min_bound.z - other.max_bound.z, 0.0f), other.min_bound.z - max_bound.z);
    return std::sqrt(dx * dx + dy * dy + dz * dz);
}

// --- Corner Points ---
void BoundingBox::GetCorners(vec3 corners[8]) const {
    corners[0] = vec3(min_bound.x, min_bound.y, min_bound.z);
    corners[1] = vec3(max_bound.x, min_bound.y, min_bound.z);
    corners[2] = vec3(min_bound.x, max_bound.y, min_bound.z);
    corners[3] = vec3(max_bound.x, max_bound.y, min_bound.z);
    corners[4] = vec3(min_bound.x, min_bound.y, max_bound.z);
    corners[5] = vec3(max_bound.x, min_bound.y, max_bound.z);
    corners[6] = vec3(min_bound.x, max_bound.y, max_bound.z);
    corners[7] = vec3(max_bound.x, max_bound.y, max_bound.z);
}

std::vector<vec3> BoundingBox::GetCorners() const {
    std::vector<vec3> corners(8);
    GetCorners(corners.data());
    return corners;
}

// --- Transformation ---
BoundingBox BoundingBox::Transform(const mat4& matrix) const {
    vec3 corners[8];
    GetCorners(corners);

    BoundingBox result;
    for (int i = 0; i < 8; ++i) {
        vec4 transformed = matrix * vec4(corners[i], 1.0f);
        result.Expand(vec3(transformed));
    }
    return result;
}

// --- Comparison ---
bool BoundingBox::operator==(const BoundingBox& other) const {
    return min_bound == other.min_bound && max_bound == other.max_bound;
}

bool BoundingBox::operator!=(const BoundingBox& other) const {
    return !(*this == other);
}

// --- Static Methods ---
BoundingBox BoundingBox::FromPoints(const std::vector<vec3>& points) {
    BoundingBox box;
    for (const vec3& point : points) {
        box.Expand(point);
    }
    return box;
}

BoundingBox BoundingBox::FromCenterAndSize(const vec3& center, const vec3& size) {
    return BoundingBox(center - size * 0.5f, center + size * 0.5f);
}

BoundingBox BoundingBox::FromCenterAndHalfSize(const vec3& center, const vec3& half_size) {
    return BoundingBox(center - half_size, center + half_size);
}

BoundingBox BoundingBox::Merge(const BoundingBox& a, const BoundingBox& b) {
    BoundingBox result;
    result.Expand(a);
    result.Expand(b);
    return result;
}

// --- Output ---
std::ostream& operator<<(std::ostream& os, const BoundingBox& box) {
    os << "BoundingBox(min=" << box.min_bound << ", max=" << box.max_bound << ")";
    return os;
}

} // namespace ygl
