#pragma once

#include "math/vec3.hpp"
#include "math/vec2.hpp"
#include <array>

namespace ygl {

class BVH;

class SphericalGBuffer {
public:
    enum class BufferType { EMITTER, RECEIVER };

    enum class TextureType {
        RADIANCE,
        POSITION,
        NORMAL,
        MATERIAL,
        DEPTH,
        COUNT
    };

    SphericalGBuffer(int resolution = 512);
    ~SphericalGBuffer();

    bool initialize();
    void cleanup();

    void beginPass(BufferType type);
    void endPass();
    void swapBuffers();

    void bindForWriting(BufferType type, int faceIndex = -1);
    void bindForReading(BufferType type);
    void bindDepthForReading(BufferType type);
    void unbind();

    unsigned int getTexture(BufferType type, TextureType texType) const;
    unsigned int getCubeMap(BufferType type) const;
    unsigned int getDepthTexture(BufferType type) const;
    unsigned int getFBO(BufferType type) const;

    void propagate(const BVH& bvh, int samples = 16);
    void setResolution(int resolution);
    int getResolution() const { return m_resolution; }
    BufferType getCurrentType() const { return m_currentType; }

    static Vec3 sphericalToCartesian(float theta, float phi);
    static void cartesianToSpherical(const Vec3& dir, float& theta, float& phi);

private:
    int m_resolution;
    BufferType m_currentType = BufferType::EMITTER;

    struct BufferTextures {
        unsigned int cubeMap = 0;
        unsigned int depthTexture = 0;
        unsigned int fbo = 0;
        std::array<unsigned int, static_cast<int>(TextureType::COUNT)> textures = {0};
    };

    BufferTextures m_emitter;
    BufferTextures m_receiver;

    unsigned int m_propagateShader = 0;

    bool createCubeMap(BufferTextures& buffer);
    bool createDepthTexture(BufferTextures& buffer);
    bool createFBO(BufferTextures& buffer);
    void deleteBuffer(BufferTextures& buffer);

    unsigned int loadShader(const std::string& vertPath, const std::string& fragPath);
    void renderFullscreenQuad();
};

} // namespace ygl

