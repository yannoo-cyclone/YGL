#include "render/bvh.hpp"
#include <chrono>
#include <algorithm>
#include <limits>

namespace ygl {

// ============================================
// Triangle Implementation
// ============================================

BoundingBox Triangle::getBoundingBox() const {
    BoundingBox box;
    box.Expand(v0);
    box.Expand(v1);
    box.Expand(v2);
    return box;
}

bool Triangle::intersect(const Ray& ray, float t_min, float t_max, HitInfo& hit) const {
    // TODO: Implement triangle intersection
    return false;
}

// ============================================
// BVHNode Implementation
// ============================================

BVHNode::BVHNode() : left(0), right(0), first_prim(0), prim_count(0) {}

// ============================================
// BVH Implementation
// ============================================

BVH::BVH() : m_rootNode(0) {}

BVH::~BVH() {
    if (!m_nodes.empty()) {
        delete[] m_nodes;
    }
}

void BVH::build(const std::vector<Triangle>& triangles) {
    if (triangles.empty()) return;

    auto startTime = std::chrono::high_resolution_clock::now();

    m_triangles = triangles;
    m_nodes.resize(triangles.size() * 2 - 1);
    m_nodes[0].bounds = BoundingBox::FromPoints({});
    for (const auto& tri : triangles) {
        m_nodes[0].bounds.Expand(tri.getBoundingBox());
    }

    std::vector<uint32_t> prim_indices(triangles.size());
    for (uint32_t i = 0; i < prim_indices.size(); ++i) {
        prim_indices[i] = i;
    }

    m_nodes[0].first_prim = 0;
    m_nodes[0].prim_count = prim_indices.size();
    m_nodes[0].left = 1;
    m_nodes[0].right = 2;

    uint32_t node_idx = 0;
    std::vector<BVHNode> temp_nodes;
    temp_nodes.push_back(m_nodes[0]);

    while (node_idx < temp_nodes.size()) {
        BVHNode& node = temp_nodes[node_idx];
        if (node.prim_count <= 2) {
            node_idx++;
            continue;
        }

        BoundingBox centroid_bounds;
        for (uint32_t i = node.first_prim; i < node.first_prim + node.prim_count; ++i) {
            uint32_t prim_idx = prim_indices[i];
            Vec3 centroid = (m_triangles[prim_idx].v0 + m_triangles[prim_idx].v1 + m_triangles[prim_idx].v2) / 3.0f;
            centroid_bounds.Expand(centroid);
        }

        uint32_t split_axis = centroid_bounds.GetMaxDimension();
        uint32_t mid = node.first_prim + node.prim_count / 2;

        if (centroid_bounds.GetMax().x == centroid_bounds.GetMin().x) {
            std::nth_element(&prim_indices[node.first_prim], &prim_indices[mid],
                &prim_indices[node.first_prim + node.prim_count],
                [&](uint32_t a, uint32_t b) {
                    return (m_triangles[a].v0.y + m_triangles[a].v1.y + m_triangles[a].v2.y) <
                           (m_triangles[b].v0.y + m_triangles[b].v1.y + m_triangles[b].v2.y);
                });
        } else if (centroid_bounds.GetMax().y == centroid_bounds.GetMin().y) {
            std::nth_element(&prim_indices[node.first_prim], &prim_indices[mid],
                &prim_indices[node.first_prim + node.prim_count],
                [&](uint32_t a, uint32_t b) {
                    return (m_triangles[a].v0.z + m_triangles[a].v1.z + m_triangles[a].v2.z) <
                           (m_triangles[b].v0.z + m_triangles[b].v1.z + m_triangles[b].v2.z);
                });
        } else {
            std::nth_element(&prim_indices[node.first_prim], &prim_indices[mid],
                &prim_indices[node.first_prim + node.prim_count],
                [&](uint32_t a, uint32_t b) {
                    return (m_triangles[a].v0.x + m_triangles[a].v1.x + m_triangles[a].v2.x) <
                           (m_triangles[b].v0.x + m_triangles[b].v1.x + m_triangles[b].v2.x);
                });
        }

        BoundingBox left_bounds, right_bounds;
        for (uint32_t i = node.first_prim; i < mid; ++i) {
            left_bounds.Expand(m_triangles[prim_indices[i]].getBoundingBox());
        }
        for (uint32_t i = mid; i < node.first_prim + node.prim_count; ++i) {
            right_bounds.Expand(m_triangles[prim_indices[i]].getBoundingBox());
        }

        uint32_t left_child = temp_nodes.size();
        temp_nodes.push_back(BVHNode());
        temp_nodes[left_child].bounds = left_bounds;
        temp_nodes[left_child].first_prim = node.first_prim;
        temp_nodes[left_child].prim_count = mid - node.first_prim;
        temp_nodes[left_child].left = 0;
        temp_nodes[left_child].right = 0;

        uint32_t right_child = temp_nodes.size();
        temp_nodes.push_back(BVHNode());
        temp_nodes[right_child].bounds = right_bounds;
        temp_nodes[right_child].first_prim = mid;
        temp_nodes[right_child].prim_count = node.first_prim + node.prim_count - mid;
        temp_nodes[right_child].left = 0;
        temp_nodes[right_child].right = 0;

        node.left = left_child;
        node.right = right_child;

        node_idx++;
    }

    m_nodes = temp_nodes;
    m_rootNode = 0;

    auto endTime = std::chrono::high_resolution_clock::now();
    m_buildTime = std::chrono::duration<float>(endTime - startTime).count();
}

float BVH::calculateSAH(const BoundingBox& box, int prim_count) const {
    float surfaceArea = box.GetSurfaceArea();
    return 2.0f * surfaceArea * prim_count;
}

bool BVH::intersect(const Ray& ray, float& t, uint32_t& prim_id) const {
    float tmin, tmax;
    if (!m_nodes[m_rootNode].bounds.Intersect(ray, tmin, tmax)) {
        return false;
    }
    return intersectNode(m_nodes[m_rootNode], ray, 0.0f, std::numeric_limits<float>::max(), t, prim_id);
}

bool BVH::intersect(const Ray& ray, float t_min, float t_max, float& t, uint32_t& prim_id) const {
    if (!m_nodes[m_rootNode].bounds.Intersect(ray, t_min, t_max, t_min, t_max)) {
        return false;
    }
    return intersectNode(m_nodes[m_rootNode], ray, t_min, t_max, t, prim_id);
}

bool BVH::intersectNode(const BVHNode& node, const Ray& ray, float t_min, float t_max, float& t, uint32_t& prim_id) const {
    if (!node.bounds.Intersect(ray, t_min, t_max, t_min, t_max)) {
        return false;
    }

    if (node.prim_count > 0) {
        for (uint32_t i = node.first_prim; i < node.first_prim + node.prim_count; ++i) {
            HitInfo hit;
            if (m_triangles[i].intersect(ray, t_min, t_max, hit)) {
                if (hit.t < t) {
                    t = hit.t;
                    prim_id = i;
                }
            }
        }
        return t < t_max;
    }

    float leftTmin, leftTmax, rightTmin, rightTmax;
    bool leftHit = m_nodes[node.left].bounds.Intersect(ray, t_min, t_max, leftTmin, leftTmax);
    bool rightHit = m_nodes[node.right].bounds.Intersect(ray, t_min, t_max, rightTmin, rightTmax);

    bool hit1 = false, hit2 = false;
    if (leftHit) {
        hit1 = intersectNode(m_nodes[node.left], ray, leftTmin, leftTmax, t, prim_id);
    }
    if (rightHit) {
        hit2 = intersectNode(m_nodes[node.right], ray, rightTmin, rightTmax, t, prim_id);
    }

    return hit1 || hit2;
}

} // namespace ygl
