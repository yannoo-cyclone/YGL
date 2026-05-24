#include "render/deferred_renderer.hpp"
#include "core/mesh.hpp"
#include "core/light.hpp"
#include <GL/glew.h>

namespace ygl {

DeferredRenderer::DeferredRenderer(int width, int height)
    : m_width(width), m_height(height), m_gbuffer(width, height) {}

DeferredRenderer::~DeferredRenderer() {
    cleanup();
}

bool DeferredRenderer::initialize() {
    if (!m_gbuffer.initialize()) return false;
    if (!createFinalTexture()) return false;
    if (!loadShaders()) return false;
    return true;
}

void DeferredRenderer::cleanup() {
    m_gbuffer.cleanup();
    if (m_finalTexture) { glDeleteTextures(1, &m_finalTexture); m_finalTexture = 0; }
    if (m_finalFBO) { glDeleteFramebuffers(1, &m_finalFBO); m_finalFBO = 0; }
    if (m_geometryShader) { glDeleteProgram(m_geometryShader); m_geometryShader = 0; }
    if (m_lightingShader) { glDeleteProgram(m_lightingShader); m_lightingShader = 0; }
    if (m_compositeShader) { glDeleteProgram(m_compositeShader); m_compositeShader = 0; }
}

void DeferredRenderer::resize(int width, int height) {
    m_width = width; m_height = height;
    m_gbuffer.resize(width, height);
    createFinalTexture();
}

bool DeferredRenderer::createFinalTexture() {
    glGenTextures(1, &m_finalTexture);
    glBindTexture(GL_TEXTURE_2D, m_finalTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_width, m_height,
                0, GL_RGBA, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenFramebuffers(1, &m_finalFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_finalFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                          GL_TEXTURE_2D, m_finalTexture, 0);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "DeferredRenderer final FBO incomplete: " << status << std::endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return false;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return true;
}

bool DeferredRenderer::loadShaders() {
    // Stub implementation - load actual shaders in a real implementation
    m_geometryShader = 0;
    m_lightingShader = 0;
    m_compositeShader = 0;
    return true;
}

void DeferredRenderer::render(const Scene& scene, const Camera& camera) {
    // Geometry pass
    renderGeometryPass(scene, camera);

    // Lighting pass
    renderLightingPass(scene, camera);

    // Composite pass
    renderCompositePass();
}

void DeferredRenderer::renderGeometryPass(const Scene& scene, const Camera& camera) {
    m_gbuffer.bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Mat4 view = camera.getViewMatrix();
    Mat4 proj = camera.getProjectionMatrix();

    // Set up shader and uniforms
    glUseProgram(m_geometryShader);
    glUniformMatrix4fv(glGetUniformLocation(m_geometryShader, "uView"), 1, GL_FALSE, &view(0,0));
    glUniformMatrix4fv(glGetUniformLocation(m_geometryShader, "uProjection"), 1, GL_FALSE, &proj(0,0));

    // Render all meshes
    for (auto& mesh : scene.getMeshes()) {
        if (mesh->isVisible()) {
            Mat4 model = mesh->getWorldMatrix();
            glUniformMatrix4fv(glGetUniformLocation(m_geometryShader, "uModel"), 1, GL_FALSE, &model(0,0));
            mesh->render();
        }
    }

    glUseProgram(0);
    m_gbuffer.unbind();
}

void DeferredRenderer::renderLightingPass(const Scene& scene, const Camera& camera) {
    // Stub implementation
}

void DeferredRenderer::renderCompositePass() {
    // Stub implementation
}

} // namespace ygl
