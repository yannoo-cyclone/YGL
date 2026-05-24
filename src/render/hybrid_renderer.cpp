#include "render/hybrid_renderer.hpp"
#include <GL/glew.h>

namespace ygl {

HybridRenderer::HybridRenderer(int width, int height, int sphericalResolution)
    : m_width(width), m_height(height),
      m_deferredRenderer(width, height),
      m_pathTracingRenderer(width, height, sphericalResolution) {}

HybridRenderer::~HybridRenderer() {
    cleanup();
}

bool HybridRenderer::initialize(const Scene& scene) {
    if (!m_deferredRenderer.initialize()) return false;
    if (!m_pathTracingRenderer.initialize(scene)) return false;
    if (!initFinalTexture()) return false;
    return true;
}

void HybridRenderer::cleanup() {
    m_deferredRenderer.cleanup();
    m_pathTracingRenderer.cleanup();
    if (m_finalTexture) { glDeleteTextures(1, &m_finalTexture); m_finalTexture = 0; }
    if (m_finalFBO) { glDeleteFramebuffers(1, &m_finalFBO); m_finalFBO = 0; }
}

bool HybridRenderer::initFinalTexture() {
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
        std::cerr << "HybridRenderer FBO incomplete: " << status << std::endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return false;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return true;
}

void HybridRenderer::render(const Scene& scene, const Camera& camera) {
    if (m_enableDeferred) {
        m_deferredRenderer.render(scene, camera);
    }

    if (m_enablePathTracing) {
        m_pathTracingRenderer.render(scene, camera);
    }

    compositeResults();
}

void HybridRenderer::compositeResults() {
    GLint currentFBO; glGetIntegerv(GL_FRAMEBUFFER_BINDING, &currentFBO);

    glBindFramebuffer(GL_FRAMEBUFFER, m_finalFBO);
    glClear(GL_COLOR_BUFFER_BIT);

    // Composite deferred + path tracing results
    // Implementation depends on your specific needs

    glBindFramebuffer(GL_FRAMEBUFFER, currentFBO);
}

} // namespace ygl
