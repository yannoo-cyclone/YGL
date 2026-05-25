#pragma once

#include "core/mesh.hpp"
#include "core/bounding_box.hpp"
#include "math/Ray.hpp"
#include <vector>
#include <memory>

namespace ygl {

class Triangle;

struct BVHNode {
    BoundingBox bounds;
    union {
        struct { uint32_t left; uint32_t right; };
        struct { uint32_t first; uint32_t count; };
    };
    bool isLeaf() const { return left >= right; }
};

class BVH {
public:
    BVH();
    ~BVH();

    void build(const Mesh& mesh);
    void build(const std::vector<Triangle>& triangles);
    void clear();

    bool intersect(const Ray& ray, float& t, uint32_t& triangleIndex) const;
    bool intersect(const Ray& ray, float tmin, float tmax, float& t, uint32_t& triangleIndex) const;

    const std::vector<BVHNode>& getNodes() const { return m_nodes; }
    const BVHNode& getRoot() const { return m_nodes[0]; }
    const std::vector<Triangle>& getTriangles() const { return m_triangles; }

    size_t getNodeCount() const { return m_nodes.size(); }
    size_t getTriangleCount() const { return m_triangles.size(); }
    float getBuildTime() const { return m_buildTime; }

private:
    std::vector<BVHNode> m_nodes;
    std::vector<Triangle> m_triangles;
    float m_buildTime = 0.0f;

    void buildBottomUp(const std::vector<Triangle>& triangles);
    float calculateSAH(const BoundingBox& box, int triangleCount) const;
    bool intersectNode(const BVHNode& node, const Ray& ray, float tmin, float tmax, float& t, uint32_t& triangleIndex) const;
    bool intersectTriangle(const Triangle& triangle, const Ray& ray, float tmin, float tmax, float& t) const;
};

struct Triangle {
    Vec3 v0, v1, v2;
    Vec3 n0, n1, n2;
    Vec2 t0, t1, t2;
    uint32_t materialId;
    uint32_t meshId;
    uint32_t faceId;

    Triangle() = default;
    Triangle(const Vec3& v0, const Vec3& v1, const Vec3& v2);

    Vec3 getCentroid() const;
    BoundingBox getBoundingBox() const;
    Vec3 getNormal() const;

    bool intersect(const Ray& ray, float tmin, float tmax, float& t, Vec2& barycentric) const;
};

} // namespace ygl

