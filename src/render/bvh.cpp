#include "render/bvh.hpp"
#include <algorithm>
#include <limits>

namespace ygl {

// Triangle implementation
Triangle::Triangle(const Vec3& v0, const Vec3& v1, const Vec3& v2)
    : v0(v0), v1(v1), v2(v2) {}

Vec3 Triangle::getCentroid() const {
    return (v0 + v1 + v2) / 3.0f;
}

BoundingBox Triangle::getBoundingBox() const {
    BoundingBox box;
    box.expand(v0); box.expand(v1); box.expand(v2);
    return box;
}

Vec3 Triangle::getNormal() const {
    return normalize(cross(v1 - v0, v2 - v0));
}

bool Triangle::intersect(const Ray& ray, float tmin, float tmax, float& t, Vec2& barycentric) const {
    Vec3 edge1 = v1 - v0;
    Vec3 edge2 = v2 - v0;
    Vec3 h = cross(ray.direction, edge2);
    float a = dot(edge1, h);

    if (a > -0.0001f && a < 0.0001f) return false;
    float f = 1.0f / a;
    Vec3 s = ray.origin - v0;
    float u = f * dot(s, h);

    if (u < 0.0f || u > 1.0f) return false;
    Vec3 q = cross(s, edge1);
    float v = f * dot(ray.direction, q);

    if (v < 0.0f || u + v > 1.0f) return false;
    float dist = f * dot(edge2, q);

    if (dist < tmin || dist > tmax) return false;
    t = dist;
    barycentric = Vec2(u, v);
    return true;
}

// BVH implementation
BVH::BVH() {}
BVH::~BVH() {}

void BVH::clear() {
    m_nodes.clear();
    m_triangles.clear();
}

void BVH::build(const Mesh& mesh) {
    std::vector<Triangle> triangles;
    const auto& positions = mesh.getPositions();
    const auto& indices = mesh.getIndices();

    for (size_t i = 0; i < indices.size(); i += 3) {
        if (i + 2 < indices.size()) {
            triangles.emplace_back(
                positions[indices[i]],
                positions[indices[i+1]],
                positions[indices[i+2]]
            );
        }
    }
    build(triangles);
}

void BVH::build(const std::vector<Triangle>& triangles) {
    auto startTime = std::chrono::high_resolution_clock::now();
    m_triangles = triangles;

    if (m_triangles.empty()) {
        m_buildTime = 0.0f;
        return;
    }

    // Build bottom-up BVH with SAH
    std::vector<BoundingBox> bounds(m_triangles.size());
    for (size_t i = 0; i < m_triangles.size(); ++i) {
        bounds[i] = m_triangles[i].getBoundingBox();
    }

    std::vector<int> indices(m_triangles.size());
    for (size_t i = 0; i < indices.size(); ++i) indices[i] = i;

    std::vector<BVHNode> nodes;
    std::vector<int> nodeIndices;

    // Build leaf nodes
    for (size_t i = 0; i < m_triangles.size(); ++i) {
        BVHNode node;
        node.bounds = bounds[i];
        node.first = static_cast<uint32_t>(i);
        node.count = 1;
        nodes.push_back(node);
        nodeIndices.push_back(static_cast<int>(i));
    }

    // Build internal nodes
    while (nodes.size() > 1) {
        float minCost = std::numeric_limits<float>::max();
        int minIndex1 = -1, minIndex2 = -1;

        for (size_t i = 0; i < nodes.size(); ++i) {
            for (size_t j = i + 1; j < nodes.size(); ++j) {
                BoundingBox combined = nodes[i].bounds;
                combined.expand(nodes[j].bounds);
                float cost = calculateSAH(combined, nodes[i].count + nodes[j].count);
                if (cost < minCost) {
                    minCost = cost;
                    minIndex1 = static_cast<int>(i);
                    minIndex2 = static_cast<int>(j);
                }
            }
        }

        if (minIndex1 == -1 || minIndex2 == -1) break;

        BVHNode parent;
        parent.bounds = nodes[minIndex1].bounds;
        parent.bounds.expand(nodes[minIndex2].bounds);
        parent.left = static_cast<uint32_t>(nodes.size());
        parent.right = static_cast<uint32_t>(nodes.size() + 1);

        nodes.push_back(parent);

        // Replace the two nodes with the parent
        if (minIndex1 < minIndex2) {
            nodes[minIndex1] = nodes.back();
            nodes.pop_back();
            nodes[minIndex2-1] = nodes.back();
            nodes.pop_back();
        } else {
            nodes[minIndex2] = nodes.back();
            nodes.pop_back();
            nodes[minIndex1-1] = nodes.back();
            nodes.pop_back();
        }
        nodes.push_back(parent);
    }

    if (!nodes.empty()) {
        m_nodes = nodes;
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    m_buildTime = std::chrono::duration<float>(endTime - startTime).count();
}

float BVH::calculateSAH(const BoundingBox& box, int triangleCount) const {
    float surfaceArea = box.getSurfaceArea();
    if (surfaceArea == 0.0f) return std::numeric_limits<float>::max();
    return 2.0f * surfaceArea * static_cast<float>(triangleCount);
}

bool BVH::intersect(const Ray& ray, float& t, uint32_t& triangleIndex) const {
    float tmin, tmax;
    return intersect(ray, 0.0f, std::numeric_limits<float>::max(), t, triangleIndex);
}

bool BVH::intersect(const Ray& ray, float tmin, float tmax, float& t, uint32_t& triangleIndex) const {
    if (m_nodes.empty()) return false;

    float closestT = tmax;
    uint32_t closestIndex = ~0u;

    std::vector<const BVHNode*> stack;
    stack.push_back(&m_nodes[0]);

    while (!stack.empty()) {
        const BVHNode* node = stack.back();
        stack.pop_back();

        if (!node->bounds.intersect(ray, tmin, tmax, tmin, tmax)) {
            continue;
        }

        if (node->isLeaf()) {
            for (uint32_t i = node->first; i < node->first + node->count; ++i) {
                float currentT;
                Vec2 barycentric;
                if (m_triangles[i].intersect(ray, tmin, closestT, currentT, barycentric)) {
                    if (currentT < closestT) {
                        closestT = currentT;
                        triangleIndex = i;
                    }
                }
            }
        } else {
            stack.push_back(&m_nodes[node->left]);
            stack.push_back(&m_nodes[node->right]);
        }
    }

    if (closestIndex != ~0u) {
        t = closestT;
        return true;
    }
    return false;
}

bool BVH::intersectNode(const BVHNode& node, const Ray& ray, float tmin, float tmax, float& t, uint32_t& triangleIndex) const {
    if (!node.isLeaf()) {
        float leftTmin, leftTmax, rightTmin, rightTmax;
        bool leftHit = m_nodes[node.left].bounds.intersect(ray, tmin, tmax, leftTmin, leftTmax);
        bool rightHit = m_nodes[node.right].bounds.intersect(ray, tmin, tmax, rightTmin, rightTmax);

        if (leftHit && rightHit) {
            if (leftTmin < rightTmin) {
                return intersectNode(m_nodes[node.left], ray, tmin, tmax, t, triangleIndex) &&
                       intersectNode(m_nodes[node.right], ray, tmin, t, t, triangleIndex);
            } else {
                return intersectNode(m_nodes[node.right], ray, tmin, tmax, t, triangleIndex) &&
                       intersectNode(m_nodes[node.left], ray, tmin, t, t, triangleIndex);
            }
        } else if (leftHit) {
            return intersectNode(m_nodes[node.left], ray, tmin, tmax, t, triangleIndex);
        } else if (rightHit) {
            return intersectNode(m_nodes[node.right], ray, tmin, tmax, t, triangleIndex);
        }
        return false;
    }

    for (uint32_t i = node.first; i < node.first + node.count; ++i) {
        float currentT;
        Vec2 barycentric;
        if (m_triangles[i].intersect(ray, tmin, tmax, currentT, barycentric)) {
            if (currentT < t) {
                t = currentT;
                triangleIndex = i;
                return true;
            }
        }
    }
    return false;
}

} // namespace ygl
