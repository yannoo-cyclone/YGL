#pragma once

#include "core/object3d.hpp"
#include "core/camera.hpp"
#include <vector>
#include <memory>

namespace ygl {

class Mesh;
class Light;

class Scene : public Object3D {
public:
    Scene(const std::string& name = "Scene");
    virtual ~Scene();

    void setActiveCamera(std::shared_ptr<Camera> camera);
    std::shared_ptr<Camera> getActiveCamera() const { return m_activeCamera; }

    void addCamera(std::shared_ptr<Camera> camera);
    void removeCamera(std::shared_ptr<Camera> camera);
    void removeCamera(size_t index);
    size_t getCameraCount() const { return m_cameras.size(); }
    std::shared_ptr<Camera> getCamera(size_t index) const;

    void addLight(std::shared_ptr<Light> light);
    void removeLight(std::shared_ptr<Light> light);
    void removeLight(size_t index);
    void clearLights();
    size_t getLightCount() const { return m_lights.size(); }
    std::shared_ptr<Light> getLight(size_t index) const;
    const std::vector<std::shared_ptr<Light>>& getLights() const { return m_lights; }

    void addMesh(std::shared_ptr<Mesh> mesh);
    void removeMesh(std::shared_ptr<Mesh> mesh);
    void removeMesh(size_t index);
    void clearMeshes();
    size_t getMeshCount() const { return m_meshes.size(); }
    std::shared_ptr<Mesh> getMesh(size_t index) const;
    const std::vector<std::shared_ptr<Mesh>>& getMeshes() const { return m_meshes; }

    void render(const Camera& camera);
    void render() { if (m_activeCamera) render(*m_activeCamera); }

    virtual void update(float deltaTime) override;

    void setBackgroundColor(const Vec3& color) { m_backgroundColor = color; }
    const Vec3& getBackgroundColor() const { return m_backgroundColor; }

private:
    std::vector<std::shared_ptr<Camera>> m_cameras;
    std::shared_ptr<Camera> m_activeCamera;

    std::vector<std::shared_ptr<Light>> m_lights;
    std::vector<std::shared_ptr<Mesh>> m_meshes;

    Vec3 m_backgroundColor = Vec3(0.1f, 0.1f, 0.1f);
};

} // namespace ygl

