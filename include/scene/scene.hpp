#pragma once
#include <string>
#include <vector>
#include <cstddef>
#include "core/object3d.hpp"
#include "core/light.hpp"
#include "core/bounding_box.hpp"
#include "math/Ray.hpp"
#include "scene/animation.hpp"

namespace ygl {

class HitInfo;

class Scene : public Object3D {
public:
    Scene();
    Scene(Object3D* root_object);
    Scene(const std::string& name = "Scene");
    virtual ~Scene();

    // Objects
    void AddObject(Object3D* object);
    void RemoveObject(Object3D* object);
    void RemoveObject(size_t index);
    void ClearObjects();
    const std::vector<Object3D*>& GetObjects() const;
    Object3D* GetObject(size_t index) const;
    size_t GetObjectCount() const;
    Object3D* FindObjectByName(const std::string& name) const;

    // Lights
    void AddLight(Light* light);
    void RemoveLight(Light* light);
    void RemoveLight(size_t index);
    void ClearLights();
    const std::vector<Light*>& GetLights() const;
    Light* GetLight(size_t index) const;
    size_t GetLightCount() const;
    Light* FindLightByName(const std::string& name) const;

    // Animations
    void AddAnimation(Animation* animation);
    void RemoveAnimation(Animation* animation);
    void RemoveAnimation(size_t index);
    void ClearAnimations();
    const std::vector<Animation*>& GetAnimations() const;
    Animation* GetAnimation(size_t index) const;
    size_t GetAnimationCount() const;

    // Root & BoundingBox
    Object3D* GetRoot() const;
    BoundingBox* GetBoundingBox() const;
    void SetRoot(Object3D* new_root);
    void UpdateBoundingBox();
    BoundingBox GetSceneBoundingBox() const;

    // Update & Intersection
    void Update(float delta_time);
    bool Intersect(const Ray& ray, HitInfo& hit_info, float t_min, float t_max) const;
    bool IntersectAny(const Ray& ray, float t_min, float t_max) const;

private:
    Object3D* root;
    BoundingBox* bounding_box;
    std::vector<Object3D*> objects;
    std::vector<Light*> lights;
    std::vector<Animation*> animations;
};

} // namespace ygl
