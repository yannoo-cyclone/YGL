#include "core/scene.hpp"
#include "core/light.hpp"
#include "core/mesh.hpp"
#include <GL/glew.h>

namespace ygl {

Scene::Scene(const std::string& name) : Object3D(name), m_backgroundColor(0.2f, 0.2f, 0.2f) {}

Scene::~Scene() {
    clearMeshes();
    clearLights();
}

// Lights
void Scene::addLight(std::shared_ptr<Light> light) {
    if (!light) return;
    addChild(std::dynamic_pointer_cast<Object3D>(light));  // static → dynamic
    m_lights.push_back(light);
}

void Scene::removeLight(std::shared_ptr<Light> light) {
    for (auto it = m_lights.begin(); it != m_lights.end(); ++it) {
        if (*it == light) {
            removeChild(std::dynamic_pointer_cast<Object3D>(light));
            m_lights.erase(it);
            return;
        }
    }
}

void Scene::removeLight(size_t index) {
    if (index < m_lights.size()) {
        removeChild(std::dynamic_pointer_cast<Object3D>(m_lights[index]));
        m_lights.erase(m_lights.begin() + index);
    }
}

void Scene::clearLights() {
    for (auto& light : m_lights) {
        removeChild(std::dynamic_pointer_cast<Object3D>(light));
    }
    m_lights.clear();
}

const std::vector<std::shared_ptr<Light>>& Scene::getLights() const {
    return m_lights;
}

// Meshes
void Scene::addMesh(std::shared_ptr<Mesh> mesh) {
    if (!mesh) return;
    addChild(std::dynamic_pointer_cast<Object3D>(mesh));
    m_meshes.push_back(mesh);
}

void Scene::removeMesh(std::shared_ptr<Mesh> mesh) {
    for (auto it = m_meshes.begin(); it != m_meshes.end(); ++it) {
        if (*it == mesh) {
            removeChild(std::dynamic_pointer_cast<Object3D>(mesh));
            m_meshes.erase(it);
            return;
        }
    }
}

void Scene::removeMesh(size_t index) {
    if (index < m_meshes.size()) {
        removeChild(std::dynamic_pointer_cast<Object3D>(m_meshes[index]));
        m_meshes.erase(m_meshes.begin() + index);
    }
}

void Scene::clearMeshes() {
    for (auto& mesh : m_meshes) {
        removeChild(std::dynamic_pointer_cast<Object3D>(mesh));
    }
    m_meshes.clear();
}

const std::vector<std::shared_ptr<Mesh>>& Scene::getMeshes() const {
    return m_meshes;
}

// Background
void Scene::setBackgroundColor(const Vec3& color) {
    m_backgroundColor = color;
}

const Vec3& Scene::getBackgroundColor() const {
    return m_backgroundColor;
}

// Render
void Scene::render(const Camera& camera) {
    glClearColor(m_backgroundColor.x, m_backgroundColor.y, m_backgroundColor.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto& mesh : m_meshes) {
        mesh->render();
    }
}

} // namespace ygl
