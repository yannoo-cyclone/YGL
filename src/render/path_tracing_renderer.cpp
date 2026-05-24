#include "render/path_tracing_renderer.hpp"
#include "core/mesh.hpp"
#include "core/light.hpp"
#include "materials/material.hpp"
#include <GL/glew.h>
#include <random>

namespace ygl {

PathTracingRenderer::PathTracingRenderer(int width, int height, int sphericalResolution)
    : m_width(width), m_height(height), m_sphericalGBuffer(sphericalResolution) {}

PathTracingRenderer::~PathTracingRenderer() {
    cleanup();
}

bool PathTracingRenderer::initialize(const Scene& scene) {
    m_scene = &scene;
    if (!m_sphericalGBuffer.initialize()) return false;
    if (!initResultTexture()) return false;
    if (!loadShaders()) return false;
    return true;
}

void PathTracingRenderer::cleanup() {
    if (m_resultTexture) { glDeleteTextures(1, &m_resultTexture); m_resultTexture = 0; }
    if (m_resultFBO) { glDeleteFramebuffers(1, &m_resultFBO); m_resultFBO = 0; }
    if (m_pathTraceShader) { glDeleteProgram(m_pathTraceShader); m_pathTraceShader = 0; }
    if (m_compositeShader) { glDeleteProgram(m_compositeShader); m_compositeShader = 0; }
    m_sphericalGBuffer.cleanup();
}

bool PathTracingRenderer::initResultTexture() {
    glGenTextures(1, &m_resultTexture);
    glBindTexture(GL_TEXTURE_2D, m_resultTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_width, m_height,
                0, GL_RGBA, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glGenFramebuffers(1, &m_resultFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_resultFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                          GL_TEXTURE_2D, m_resultTexture, 0);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "PathTracingRenderer FBO incomplete: " << status << std::endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return false;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return true;
}

bool PathTracingRenderer::loadShaders() {
    m_pathTraceShader = 0;
    m_compositeShader = 0;
    return true;
}

void PathTracingRenderer::render(const Scene& scene, const Camera& camera) {
    m_scene = &scene;
    m_camera = &camera;

    initializeEmitterBuffer(scene);

    for (int pass = 0; pass < m_passCount; pass++) {
        renderPass(scene, camera, pass);
        if (m_enableDenoising && pass > 0) {
            applyDenoising();
        }
    }

    compositeFinalImage();
}

void PathTracingRenderer::renderPass(const Scene& scene, const Camera& camera, int passIndex) {
    m_sphericalGBuffer.bindForReading(SphericalGBuffer::BufferType::EMITTER);
    m_sphericalGBuffer.bindDepthForReading(SphericalGBuffer::BufferType::EMITTER);

    m_sphericalGBuffer.bindForWriting(SphericalGBuffer::BufferType::RECEIVER);

    glUseProgram(m_pathTraceShader);
    glUniform1i(glGetUniformLocation(m_pathTraceShader, "uSamples"), m_samples);
    glUniform1i(glGetUniformLocation(m_pathTraceShader, "uPassIndex"), passIndex);

    for (int face = 0; face < 6; face++) {
        m_sphericalGBuffer.bindForWriting(SphericalGBuffer::BufferType::RECEIVER, face);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUniform1i(glGetUniformLocation(m_pathTraceShader, "uFaceIndex"), face);
        m_sphericalGBuffer.renderFullscreenQuad();
    }

    m_sphericalGBuffer.swapBuffers();
    glUseProgram(0);
    m_sphericalGBuffer.unbind();
}

void PathTracingRenderer::initializeEmitterBuffer(const Scene& scene) {
    GLint currentFBO; glGetIntegerv(GL_FRAMEBUFFER_BINDING, &currentFBO);
    m_sphericalGBuffer.bindForWriting(SphericalGBuffer::BufferType::EMITTER);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, currentFBO);
}

void PathTracingRenderer::compositeFinalImage() {
    GLint currentFBO; glGetIntegerv(GL_FRAMEBUFFER_BINDING, &currentFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_resultFBO);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(m_compositeShader);
    m_sphericalGBuffer.bindForReading(SphericalGBuffer::BufferType::RECEIVER);
    glUniform1i(glGetUniformLocation(m_compositeShader, "uRadiosityMap"), 0);
    m_sphericalGBuffer.renderFullscreenQuad();

    glUseProgram(0);
    glBindFramebuffer(GL_FRAMEBUFFER, currentFBO);
    m_sphericalGBuffer.unbind();
}

void PathTracingRenderer::applyDenoising() {
    // Stub for denoising implementation
}

void PathTracingRenderer::setSphericalResolution(int resolution) {
    m_sphericalGBuffer.setResolution(resolution);
}

Vec3 PathTracingRenderer::sampleHemisphere(const Vec3& normal, float u1, float u2) const {
    float theta = 2.0f * 3.14159265359f * u1;
    float phi = std::acos(2.0f * u2 - 1.0f);
    Vec3 sampleDir = SphericalGBuffer::sphericalToCartesian(theta, phi);

    Vec3 tangent = normalize(cross(normal, Vec3(0.0f, 1.0f, 0.0f)));
    if (tangent.isZero()) tangent = Vec3(1.0f, 0.0f, 0.0f);
    Vec3 bitangent = cross(normal, tangent);
    Mat4 TBN = Mat4::identity();
    TBN(0,0) = tangent.x; TBN(0,1) = tangent.y; TBN(0,2) = tangent.z;
    TBN(1,0) = bitangent.x; TBN(1,1) = bitangent.y; TBN(1,2) = bitangent.z;
    TBN(2,0) = normal.x; TBN(2,1) = normal.y; TBN(2,2) = normal.z;

    return normalize(TBN * sampleDir);
}

Vec3 PathTracingRenderer::importanceSampleGGX(const Vec3& normal, float roughness, Vec3& wi, float& pdf) const {
    float alpha = roughness * roughness;
    float phi = 2.0f * 3.14159265359f * (rand() / (float)RAND_MAX);

    float xi1 = rand() / (float)RAND_MAX;
    float xi2 = rand() / (float)RAND_MAX;
    float slopeX = alpha * std::sqrt(xi1) / std::sqrt(1.0f - xi1);
    float slopeY = alpha * xi2 / std::sqrt(1.0f - xi1);

    float A = -slopeX * std::sin(phi) - slopeY * std::cos(phi);
    float B = slopeX * std::cos(phi) - slopeY * std::sin(phi);
    float C = 1.0f;
    float denom = A*A + B*B + C*C;
    Vec3 microNormal = Vec3(-A / std::sqrt(denom), -B / std::sqrt(denom), C / std::sqrt(denom));

    Vec3 tangent = normalize(cross(normal, Vec3(0.0f, 1.0f, 0.0f)));
    if (tangent.isZero()) tangent = Vec3(1.0f, 0.0f, 0.0f);
    Vec3 bitangent = cross(normal, tangent);
    Mat3 TBN(tangent, bitangent, normal);
    wi = TBN * microNormal;

    float cosTheta = dot(normal, wi);
    float cosTheta2 = cosTheta * cosTheta;
    float sinTheta2 = 1.0f - cosTheta2;
    float tanTheta2 = sinTheta2 / cosTheta2;
    float D = alpha * alpha / (3.14159265359f * (cosTheta2 * cosTheta2 * (alpha * alpha + tanTheta2) * (alpha * alpha + tanTheta2)));
    pdf = D * std::abs(dot(normal, wi));

    return wi;
}

Vec3 PathTracingRenderer::evaluateBRDF(const Vec3& wi, const Vec3& wo, const Vec3& normal, const Material& material) const {
    float dotNV = dot(normal, wi);
    float dotNL = dot(normal, wo);
    if (dotNV <= 0.0f || dotNL <= 0.0f) return Vec3(0.0f);

    Vec3 half = normalize(wi + wo);
    float dotNH = dot(normal, half);

    Vec3 F = fresnelSchlick(wi, normal, material.getIOR());
    float alpha = material.getRoughness() * material.getRoughness();
    float alpha2 = alpha * alpha;
    float NH2 = dotNH * dotNH;
    float denom = NH2 * (alpha2 - 1.0f) + 1.0f;
    float D = alpha2 / (3.14159265359f * denom * denom);
    float G = geometrySmith(dotNV, dotNL, material.getRoughness());

    Vec3 numerator = F * G * D;
    float denominator = 4.0f * std::max(dotNV, 0.001f) * std::max(dotNL, 0.001f);
    Vec3 specular = numerator / std::max(denominator, 0.001f);

    Vec3 diffuse = material.getAlbedo() / 3.14159265359f;
    return std::lerp(diffuse, specular, material.getMetallic());
}

Vec3 PathTracingRenderer::fresnelSchlick(const Vec3& wi, const Vec3& normal, float ior) const {
    float cosTheta = std::max(dot(wi, normal), 0.0f);
    Vec3 F0 = Vec3((ior - 1.0f) / (ior + 1.0f));
    F0 = F0 * F0;
    return F0 + (Vec3(1.0f) - F0) * std::pow(1.0f - cosTheta, 5.0f);
}

float PathTracingRenderer::geometrySmith(float dotNV, float dotNL, float roughness) const {
    float alpha = roughness * roughness;
    float k = (dotNV + alpha) * (dotNV + alpha);
    float G1V = dotNV / std::sqrt(k);
    k = (dotNL + alpha) * (dotNL + alpha);
    float G1L = dotNL / std::sqrt(k);
    return G1V * G1L;
}

bool PathTracingRenderer::russianRoulette(float threshold) const {
    return (rand() / (float)RAND_MAX) >= threshold;
}

} // namespace ygl
