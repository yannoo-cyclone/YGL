#include "core/scene.hpp"
#include "core/camera.hpp"
#include "core/mesh.hpp"
#include "core/light.hpp"

namespace ygl {

Scene::Scene(const std::string& name) : Object3D(name) {}

Scene::~Scene() {
    clearMeshes();
    clearLights();
    m_cameras.clear();
}

// Camera management
void Scene::setActiveCamera(std::shared_ptr<Camera> camera) {
    if (camera) {
        bool found = false;
        for (auto& cam : m_cameras) {
            if (cam == camera) { found = true; break; }
        }
        if (!found) addCamera(camera);
    }
    m_activeCamera = camera;
}

void Scene::addCamera(std::shared_ptr<Camera> camera) {
    if (!camera) return;
    auto currentParent = camera->getParent();
    if (currentParent) currentParent->removeChild(camera);
    addChild(camera); m_cameras.push_back(camera);
    if (!m_activeCamera) m_activeCamera = camera;
}

void Scene::removeCamera(std::shared_ptr<Camera> camera) {
    for (auto it = m_cameras.begin(); it != m_cameras.end(); ++it) {
        if (*it == camera) {
            if (m_activeCamera == camera) m_activeCamera = nullptr;
            removeChild(camera); m_cameras.erase(it); return;
        }
    }
}

void Scene::removeCamera(size_t index) {
    if (index < m_cameras.size()) {
        if (m_activeCamera == m_cameras[index]) m_activeCamera = nullptr;
        removeChild(m_cameras[index]); m_cameras.erase(m_cameras.begin() + index);
    }
}

std::shared_ptr<Camera> Scene::getCamera(size_t index) const {
    return index < m_cameras.size() ? m_cameras[index] : nullptr;
}

// Light management
void Scene::addLight(std::shared_ptr<Light> light) {
    if (!light) return; addChild(light); m_lights.push_back(light);
}
void Scene::removeLight(std::shared_ptr<Light> light) {
    for (auto it = m_lights.begin(); it != m_lights.end(); ++it) {
        if (*it == light) { removeChild(light); m_lights.erase(it); return; }
    }
}
void Scene::removeLight(size_t index) {
    if (index < m_lights.size()) { removeChild(m_lights[index]); m_lights.erase(m_lights.begin() + index); }
}
void Scene::clearLights() {
    for (auto& light : m_lights) removeChild(light);
    m_lights.clear();
}
std::shared_ptr<Light> Scene::getLight(size_t index) const {
    return index < m_lights.size() ? m_lights[index] : nullptr;
}

// Mesh management
void Scene::addMesh(std::shared_ptr<Mesh> mesh) {
    if (!mesh) return; addChild(mesh); m_meshes.push_back(mesh);
}
void Scene::removeMesh(std::shared_ptr<Mesh> mesh) {
    for (auto it = m_meshes.begin(); it != m_meshes.end(); ++it) {
        if (*it == mesh) { removeChild(mesh); m_meshes.erase(it); return; }
    }
}
void Scene::removeMesh(size_t index) {
    if (index < m_meshes.size()) { removeChild(m_meshes[index]); m_meshes.erase(m_meshes.begin() + index); }
}
void Scene::clearMeshes() {
    for (auto& mesh : m_meshes) removeChild(mesh);
    m_meshes.clear();
}
std::shared_ptr<Mesh> Scene::getMesh(size_t index) const {
    return index < m_meshes.size() ? m_meshes[index] : nullptr;
}

// Render
void Scene::render(const Camera& camera) {
    glClearColor(m_backgroundColor.x, m_backgroundColor.y, m_backgroundColor.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto& mesh : m_meshes) {
        if (mesh->isVisible()) mesh->render();
    }
}

// Update
void Scene::update(float deltaTime) {
    Object3D::update(deltaTime);
}

} // namespace ygl
