#pragma once

#include "core/object3d.hpp"
#include "core/mesh.hpp"
#include "core/light.hpp"
#include "core/camera.hpp"  // AJOUTÉ
#include "math/vec3.hpp"
#include <vector>
#include <memory>

namespace ygl {

class Scene : public Object3D {
public:
    Scene(const std::string& name = "Scene");
    ~Scene();

    // Lights
    void addLight(std::shared_ptr<Light> light);
    void removeLight(std::shared_ptr<Light> light);
    void removeLight(size_t index);
    void clearLights();
    const std::vector<std::shared_ptr<Light>>& getLights() const;

    // Meshes
    void addMesh(std::shared_ptr<Mesh> mesh);
    void removeMesh(std::shared_ptr<Mesh> mesh);
    void removeMesh(size_t index);
    void clearMeshes();
    const std::vector<std::shared_ptr<Mesh>>& getMeshes() const;

    // Background
    void setBackgroundColor(const Vec3& color);
    const Vec3& getBackgroundColor() const;

    // Render
    void render(const Camera& camera);  // Camera est maintenant inclus

private:
    std::vector<std::shared_ptr<Light>> m_lights;
    std::vector<std::shared_ptr<Mesh>> m_meshes;
    Vec3 m_backgroundColor;
};

} // namespace ygl
