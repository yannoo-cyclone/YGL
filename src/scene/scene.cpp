#include "scene/scene.hpp"
#include "core/object3d.hpp"
#include "core/mesh.hpp"
#include "core/bounding_box.hpp"
#include "scene/light.hpp"
#include "scene/animation.hpp"
#include <algorithm>

namespace ygl {

// --- Constructor/Destructor ---
Scene::Scene() : root(nullptr) {
    root = new Object3D();
    bounding_box = new BoundingBox();
}

Scene::Scene(Object3D* root_object) : root(root_object) {
    if (!root) {
        root = new Object3D();
    }
    bounding_box = new BoundingBox();
}

Scene::~Scene() {
    for (Object3D* object : objects) {
        delete object;
    }
    objects.clear();

    for (Light* light : lights) {
        delete light;
    }
    lights.clear();

    for (Animation* animation : animations) {
        delete animation;
    }
    animations.clear();

    delete root;
    delete bounding_box;
}

// --- Object Management ---
void Scene::AddObject(Object3D* object) {
    if (object) {
        objects.push_back(object);
        object->SetScene(this);
        UpdateBoundingBox();
    }
}

void Scene::RemoveObject(Object3D* object) {
    auto it = std::find(objects.begin(), objects.end(), object);
    if (it != objects.end()) {
        (*it)->SetScene(nullptr);
        objects.erase(it);
        UpdateBoundingBox();
    }
}

void Scene::RemoveObject(size_t index) {
    if (index < objects.size()) {
        objects[index]->SetScene(nullptr);
        objects.erase(objects.begin() + index);
        UpdateBoundingBox();
    }
}

void Scene::ClearObjects() {
    for (Object3D* object : objects) {
        object->SetScene(nullptr);
        delete object;
    }
    objects.clear();
    UpdateBoundingBox();
}

// --- Light Management ---
void Scene::AddLight(Light* light) {
    if (light) {
        lights.push_back(light);
        light->SetScene(this);
    }
}

void Scene::RemoveLight(Light* light) {
    auto it = std::find(lights.begin(), lights.end(), light);
    if (it != lights.end()) {
        (*it)->SetScene(nullptr);
        lights.erase(it);
    }
}

void Scene::RemoveLight(size_t index) {
    if (index < lights.size()) {
        lights[index]->SetScene(nullptr);
        lights.erase(lights.begin() + index);
    }
}

void Scene::ClearLights() {
    for (Light* light : lights) {
        light->SetScene(nullptr);
        delete light;
    }
    lights.clear();
}

// --- Animation Management ---
void Scene::AddAnimation(Animation* animation) {
    if (animation) {
        animations.push_back(animation);
    }
}

void Scene::RemoveAnimation(Animation* animation) {
    auto it = std::find(animations.begin(), animations.end(), animation);
    if (it != animations.end()) {
        animations.erase(it);
    }
}

void Scene::RemoveAnimation(size_t index) {
    if (index < animations.size()) {
        animations.erase(animations.begin() + index);
    }
}

void Scene::ClearAnimations() {
    for (Animation* animation : animations) {
        delete animation;
    }
    animations.clear();
}

// --- Accessors ---
const std::vector<Object3D*>& Scene::GetObjects() const {
    return objects;
}

const std::vector<Light*>& Scene::GetLights() const {
    return lights;
}

const std::vector<Animation*>& Scene::GetAnimations() const {
    return animations;
}

Object3D* Scene::GetRoot() const {
    return root;
}

BoundingBox* Scene::GetBoundingBox() const {
    return bounding_box;
}

size_t Scene::GetObjectCount() const {
    return objects.size();
}

size_t Scene::GetLightCount() const {
    return lights.size();
}

size_t Scene::GetAnimationCount() const {
    return animations.size();
}

Object3D* Scene::GetObject(size_t index) const {
    return (index < objects.size()) ? objects[index] : nullptr;
}

Light* Scene::GetLight(size_t index) const {
    return (index < lights.size()) ? lights[index] : nullptr;
}

Animation* Scene::GetAnimation(size_t index) const {
    return (index < animations.size()) ? animations[index] : nullptr;
}

// --- Bounding Box ---
void Scene::UpdateBoundingBox() {
    if (objects.empty()) {
        bounding_box->Reset();
        return;
    }

    BoundingBox new_box;
    for (Object3D* object : objects) {
        if (object && object->GetMesh()) {
            BoundingBox obj_box = object->GetMesh()->GetBoundingBox();
            mat4 transform = object->GetTransformMatrix();

            vec3 corners[8];
            obj_box.GetCorners(corners);

            for (int i = 0; i < 8; ++i) {
                vec4 transformed = transform * vec4(corners[i], 1.0f);
                new_box.Expand(vec3(transformed));
            }
        }
    }
    *bounding_box = new_box;
}

BoundingBox Scene::GetSceneBoundingBox() const {
    return *bounding_box;
}

// --- Update ---
void Scene::Update(float delta_time) {
    for (Animation* animation : animations) {
        if (animation) {
            animation->Update(delta_time);
        }
    }

    for (Object3D* object : objects) {
        if (object) {
            object->Update(delta_time);
        }
    }

    for (Light* light : lights) {
        if (light) {
            light->Update(delta_time);
        }
    }
}

// --- Ray Intersection ---
bool Scene::Intersect(const Ray& ray, HitInfo& hit_info, float t_min, float t_max) const {
    bool hit = false;
    HitInfo closest_hit;
    closest_hit.t = t_max;

    for (Object3D* object : objects) {
        if (object && object->GetMesh()) {
            HitInfo temp_hit;
            if (object->Intersect(ray, temp_hit, t_min, closest_hit.t)) {
                hit = true;
                closest_hit = temp_hit;
                closest_hit.object = object;
            }
        }
    }

    if (hit) {
        hit_info = closest_hit;
    }

    return hit;
}

bool Scene::IntersectAny(const Ray& ray, float t_min, float t_max) const {
    for (Object3D* object : objects) {
        if (object && object->GetMesh()) {
            if (object->IntersectAny(ray, t_min, t_max)) {
                return true;
            }
        }
    }
    return false;
}

// --- Utility Functions ---
Object3D* Scene::FindObjectByName(const std::string& name) const {
    for (Object3D* object : objects) {
        if (object && object->GetName() == name) {
            return object;
        }
    }
    return nullptr;
}

Light* Scene::FindLightByName(const std::string& name) const {
    for (Light* light : lights) {
        if (light && light->GetName() == name) {
            return light;
        }
    }
    return nullptr;
}

void Scene::SetRoot(Object3D* new_root) {
    if (root) {
        delete root;
    }
    root = new_root;
    if (root) {
        root->SetScene(this);
    }
}

} // namespace ygl
