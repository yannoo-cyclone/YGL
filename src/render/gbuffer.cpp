#include "render/gbuffer.hpp"
#include <GL/glew.h>
#include <iostream>

namespace ygl {

GBuffer::GBuffer(int width, int height) : m_width(width), m_height(height) {
    for (int i = 0; i < static_cast<int>(TextureType::COUNT); ++i) {
        m_textures[i] = 0;
    }
}

GBuffer::~GBuffer() {
    cleanup();
}

bool GBuffer::initialize() {
    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    createTextures();

    glGenTextures(1, &m_depthTexture);
    glBindTexture(GL_TEXTURE_2D, m_depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, m_width, m_height,
                0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                          GL_TEXTURE_2D, m_depthTexture, 0);

    GLenum drawBuffers[] = {
        GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2,
        GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5,
        GL_COLOR_ATTACHMENT6
    };
    glDrawBuffers(7, drawBuffers);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "GBuffer: Framebuffer incomplete! Status: " << status << std::endl;
        cleanup(); return false;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return true;
}

void GBuffer::cleanup() {
    if (m_fbo) { glDeleteFramebuffers(1, &m_fbo); m_fbo = 0; }
    deleteTextures();
    if (m_depthTexture) { glDeleteTextures(1, &m_depthTexture); m_depthTexture = 0; }
}

void GBuffer::resize(int width, int height) {
    if (m_width == width && m_height == height) return;
    m_width = width; m_height = height;

    GLint currentFBO; glGetIntegerv(GL_FRAMEBUFFER_BINDING, &currentFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    for (int i = 0; i < static_cast<int>(TextureType::COUNT); ++i) {
        if (m_textures[i]) {
            glBindTexture(GL_TEXTURE_2D, m_textures[i]);
            GLenum internalFormat;
            switch (static_cast<TextureType>(i)) {
                case TextureType::POSITION:
                case TextureType::NORMAL:
                case TextureType::MATERIAL: internalFormat = GL_RGB32F; break;
                case TextureType::ALBEDO: internalFormat = GL_RGB8; break;
                case TextureType::TEXCOORD: internalFormat = GL_RG16F; break;
                case TextureType::OBJECT_ID:
                case TextureType::FACE_ID: internalFormat = GL_R32UI; break;
                default: internalFormat = GL_RGB8; break;
            }
            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_width, m_height,
                        0, internalFormat == GL_RGB8 ? GL_RGB :
                        internalFormat == GL_RG16F ? GL_RG :
                        internalFormat == GL_R32UI ? GL_RED_INTEGER : GL_RGB,
                        internalFormat == GL_R32UI ? GL_UNSIGNED_INT : GL_FLOAT, nullptr);
        }
    }

    if (m_depthTexture) {
        glBindTexture(GL_TEXTURE_2D, m_depthTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, m_width, m_height,
                    0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, currentFBO);
}

void GBuffer::bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glViewport(0, 0, m_width, m_height);
}

void GBuffer::bindForReading() const {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);
}

void GBuffer::bindForWriting() const {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
}

void GBuffer::unbind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned int GBuffer::getTexture(TextureType type) const {
    return m_textures[static_cast<int>(type)];
}

unsigned int GBuffer::getDepthTexture() const {
    return m_depthTexture;
}

void GBuffer::renderToScreen() {
    GLint currentFBO; glGetIntegerv(GL_FRAMEBUFFER_BINDING, &currentFBO);
    GLint currentViewport[4]; glGetIntegerv(GL_VIEWPORT, currentViewport);

    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    glOrtho(0, m_width, 0, m_height, -1, 1);
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();

    glDisable(GL_DEPTH_TEST);

    float quadSize = 1.0f / 3.0f;
    for (int i = 0; i < static_cast<int>(TextureType::COUNT); ++i) {
        if (!m_textures[i]) continue;
        int row = i / 3; int col = i % 3;
        float x = col * quadSize; float y = 1.0f - (row + 1) * quadSize;
        glBindTexture(GL_TEXTURE_2D, m_textures[i]);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(x, y);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(x + quadSize, y);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(x + quadSize, y + quadSize);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(x, y + quadSize);
        glEnd();
    }

    glBindTexture(GL_TEXTURE_2D, m_depthTexture);
    float x = 0.666f; float y = 0.0f;
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(x, y);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(x + quadSize, y);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(x + quadSize, y + quadSize);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(x, y + quadSize);
    glEnd();

    glEnable(GL_DEPTH_TEST);
    glBindTexture(GL_TEXTURE_2D, 0);
    glViewport(currentViewport[0], currentViewport[1], currentViewport[2], currentViewport[3]);
    glBindFramebuffer(GL_FRAMEBUFFER, currentFBO);
}

void GBuffer::createTextures() {
    // POSITION (RGB32F)
    glGenTextures(1, &m_textures[static_cast<int>(TextureType::POSITION)]);
    glBindTexture(GL_TEXTURE_2D, m_textures[static_cast<int>(TextureType::POSITION)]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, m_width, m_height, 0, GL_RGB, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                          GL_TEXTURE_2D, m_textures[static_cast<int>(TextureType::POSITION)], 0);

    // NORMAL (RGB32F)
    glGenTextures(1, &m_textures[static_cast<int>(TextureType::NORMAL)]);
    glBindTexture(GL_TEXTURE_2D, m_textures[static_cast<int>(TextureType::NORMAL)]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, m_width, m_height, 0, GL_RGB, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1,
                          GL_TEXTURE_2D, m_textures[static_cast<int>(TextureType::NORMAL)], 0);

    // ALBEDO (RGB8)
    glGenTextures(1, &m_textures[static_cast<int>(TextureType::ALBEDO)]);
    glBindTexture(GL_TEXTURE_2D, m_textures[static_cast<int>(TextureType::ALBEDO)]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2,
                          GL_TEXTURE_2D, m_textures[static_cast<int>(TextureType::ALBEDO)], 0);

    // MATERIAL (RGBA32F)
    glGenTextures(1, &m_textures[static_cast<int>(TextureType::MATERIAL)]);
    glBindTexture(GL_TEXTURE_2D, m_textures[static_cast<int>(TextureType::MATERIAL)]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_width, m_height, 0, GL_RGBA, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3,
                          GL_TEXTURE_2D, m_textures[static_cast<int>(TextureType::MATERIAL)], 0);

    // TEXCOORD (RG16F)
    glGenTextures(1, &m_textures[static_cast<int>(TextureType::TEXCOORD)]);
    glBindTexture(GL_TEXTURE_2D, m_textures[static_cast<int>(TextureType::TEXCOORD)]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, m_width, m_height, 0, GL_RG, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4,
                          GL_TEXTURE_2D, m_textures[static_cast<int>(TextureType::TEXCOORD)], 0);

    // OBJECT_ID (R32UI)
    glGenTextures(1, &m_textures[static_cast<int>(TextureType::OBJECT_ID)]);
    glBindTexture(GL_TEXTURE_2D, m_textures[static_cast<int>(TextureType::OBJECT_ID)]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, m_width, m_height, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT5,
                          GL_TEXTURE_2D, m_textures[static_cast<int>(TextureType::OBJECT_ID)], 0);

    // FACE_ID (R32UI)
    glGenTextures(1, &m_textures[static_cast<int>(TextureType::FACE_ID)]);
    glBindTexture(GL_TEXTURE_2D, m_textures[static_cast<int>(TextureType::FACE_ID)]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, m_width, m_height, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT6,
                          GL_TEXTURE_2D, m_textures[static_cast<int>(TextureType::FACE_ID)], 0);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void GBuffer::deleteTextures() {
    for (int i = 0; i < static_cast<int>(TextureType::COUNT); ++i) {
        if (m_textures[i]) { glDeleteTextures(1, &m_textures[i]); m_textures[i] = 0; }
    }
}

} // namespace ygl
