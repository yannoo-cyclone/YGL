#pragma once

#include "math/vec3.hpp"
#include "math/vec2.hpp"
#include <vector>

namespace ygl {

class GBuffer {
public:
    enum class TextureType {
        POSITION,
        NORMAL,
        ALBEDO,
        MATERIAL,
        TEXCOORD,
        OBJECT_ID,
        FACE_ID,
        DEPTH,
        COUNT
    };

    GBuffer(int width, int height);
    ~GBuffer();

    bool initialize();
    void cleanup();
    void resize(int width, int height);

    void bind() const;
    void bindForReading() const;
    void bindForWriting() const;
    void unbind() const;

    unsigned int getTexture(TextureType type) const;
    unsigned int getDepthTexture() const;
    unsigned int getFBO() const { return m_fbo; }

    void renderToScreen();
    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }

private:
    int m_width;
    int m_height;

    unsigned int m_fbo = 0;
    unsigned int m_textures[static_cast<int>(TextureType::COUNT)] = {0};
    unsigned int m_depthTexture = 0;

    void createTextures();
    void deleteTextures();
};

} // namespace ygl

