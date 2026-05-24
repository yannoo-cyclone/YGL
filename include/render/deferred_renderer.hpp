#pragma once

#include "render/gbuffer.hpp"
#include "core/scene.hpp"
#include "core/camera.hpp"
#include <memory>

namespace ygl {

class DeferredRenderer {
public:
    DeferredRenderer(int width, int height);
    ~DeferredRenderer();

    bool initialize();
    void cleanup();
    void resize(int width, int height);

    void render(const Scene& scene, const Camera& camera);

    GBuffer& getGBuffer() { return m_gbuffer; }
    const GBuffer& getGBuffer() const { return m_gbuffer; }

    unsigned int getFinalTexture() const { return m_finalTexture; }

    void setEnableShadows(bool enable) { m_enableShadows = enable; }
    bool isShadowsEnabled() const { return m_enableShadows; }

    void setEnableAmbientOcclusion(bool enable) { m_enableAmbientOcclusion = enable; }
    bool isAmbientOcclusionEnabled() const { return m_enableAmbientOcclusion; }

    void setEnableGlobalIllumination(bool enable) { m_enableGlobalIllumination = enable; }
    bool isGlobalIlluminationEnabled() const { return m_enableGlobalIllumination; }

private:
    int m_width;
    int m_height;

    GBuffer m_gbuffer;
    unsigned int m_finalTexture = 0;
    unsigned int m_finalFBO = 0;

    unsigned int m_geometryShader = 0;
    unsigned int m_lightingShader = 0;
    unsigned int m_compositeShader = 0;

    bool m_enableShadows = true;
    bool m_enableAmbientOcclusion = false;
    bool m_enableGlobalIllumination = false;

    void renderGeometryPass(const Scene& scene, const Camera& camera);
    void renderLightingPass(const Scene& scene, const Camera& camera);
    void renderCompositePass();
    void createFinalTexture();
    bool loadShaders();
};

} // namespace ygl

