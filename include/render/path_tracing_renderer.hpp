#pragma once

#include "render/spherical_renderer.hpp"
#include "render/bvh.hpp"
#include "core/scene.hpp"
#include "core/camera.hpp"

namespace ygl {

class PathTracingRenderer {
public:
    PathTracingRenderer(int width = 1280, int height = 720, int sphericalResolution = 512);
    ~PathTracingRenderer();

    bool initialize(const Scene& scene);
    void cleanup();

    void render(const Scene& scene, const Camera& camera);
    void renderPass(const Scene& scene, const Camera& camera, int passIndex);

    void setSampleCount(int samples) { m_samples = samples; }
    int getSampleCount() const { return m_samples; }

    void setMaxBounces(int bounces) { m_maxBounces = bounces; }
    int getMaxBounces() const { return m_maxBounces; }

    void setPassCount(int passes) { m_passCount = passes; }
    int getPassCount() const { return m_passCount; }

    void setSphericalResolution(int resolution);
    int getSphericalResolution() const { return m_sphericalGBuffer.getResolution(); }

    void setEnableDenoising(bool enable) { m_enableDenoising = enable; }
    bool isDenoisingEnabled() const { return m_enableDenoising; }

    void setEnableDirectLighting(bool enable) { m_enableDirectLighting = enable; }
    bool isDirectLightingEnabled() const { return m_enableDirectLighting; }

    unsigned int getResultTexture() const { return m_resultTexture; }
    SphericalGBuffer& getSphericalGBuffer() { return m_sphericalGBuffer; }
    const BVH& getBVH() const { return m_bvh; }

private:
    int m_width;
    int m_height;

    SphericalGBuffer m_sphericalGBuffer;
    BVH m_bvh;

    unsigned int m_resultTexture = 0;
    unsigned int m_resultFBO = 0;

    int m_samples = 32;
    int m_maxBounces = 5;
    int m_passCount = 4;

    bool m_enableDenoising = true;
    bool m_enableDirectLighting = true;

    unsigned int m_pathTraceShader = 0;
    unsigned int m_compositeShader = 0;

    const Scene* m_scene = nullptr;
    const Camera* m_camera = nullptr;

    bool initResultTexture();
    bool loadShaders();

    void initializeEmitterBuffer(const Scene& scene);
    void compositeFinalImage();
    void applyDenoising();

    Vec3 sampleHemisphere(const Vec3& normal, float u1, float u2) const;
    Vec3 importanceSampleGGX(const Vec3& normal, float roughness, Vec3& wi, float& pdf) const;
    Vec3 evaluateBRDF(const Vec3& wi, const Vec3& wo, const Vec3& normal,
                     const class Material& material) const;
    Vec3 fresnelSchlick(const Vec3& wi, const Vec3& normal, float ior) const;
    float geometrySmith(float dotNV, float dotNL, float roughness) const;
    bool russianRoulette(float threshold) const;
};

} // namespace ygl

