#include "materials/texture.hpp"
#include "stb_image.hpp"
#include <GL/glew.h>
#include <algorithm>

namespace ygl {

Texture::Texture(Type type, const std::string& name)
    : m_type(type), m_name(name), m_id(0), m_width(0), m_height(0), m_format(Format::RGBA8) {
    glGenTextures(1, &m_id);
}

Texture::~Texture() {
    if (m_id != 0) glDeleteTextures(1, &m_id);
}

bool Texture::loadFromFile(const std::string& filename) {
    int w, h, c;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(filename.c_str(), &w, &h, &c, 0);
    if (!data) return false;

    m_width = w;
    m_height = h;
    m_filename = filename;

    GLenum gl_format;
    if (c == 1) { m_format = Format::R8; gl_format = GL_RED; }
    else if (c == 2) { m_format = Format::RG8; gl_format = GL_RG; }
    else if (c == 3) { m_format = Format::RGB8; gl_format = GL_RGB; }
    else { m_format = Format::RGBA8; gl_format = GL_RGBA; }

    glBindTexture(GL_TEXTURE_2D, m_id);
    glTexImage2D(GL_TEXTURE_2D, 0, getGLInternalFormat(m_format), m_width, m_height, 0, gl_format, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
    return true;
}

bool Texture::create(int width, int height, Format format) {
    m_width = width;
    m_height = height;
    m_format = format;

    glBindTexture(GL_TEXTURE_2D, m_id);
    glTexImage2D(GL_TEXTURE_2D, 0, getGLInternalFormat(format), width, height, 0, getGLFormat(format), getGLType(format), nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    return true;
}

bool Texture::create(int width, int height, Format format, const void* data) {
    m_width = width;
    m_height = height;
    m_format = format;

    glBindTexture(GL_TEXTURE_2D, m_id);
    glTexImage2D(GL_TEXTURE_2D, 0, getGLInternalFormat(format), width, height, 0, getGLFormat(format), getGLType(format), data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    return true;
}

void Texture::bind(int textureUnit) const {
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, m_id);
}

void Texture::unbind(int textureUnit) {
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::setWrapMode(WrapMode mode) {
    setWrapMode(mode, mode);
}

void Texture::setWrapMode(WrapMode s, WrapMode t) {
    glBindTexture(GL_TEXTURE_2D, m_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(s));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(t));
    m_wrapS = s;
    m_wrapT = t;
}

void Texture::setFilterMode(FilterMode minFilter, FilterMode magFilter) {
    glBindTexture(GL_TEXTURE_2D, m_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(minFilter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(magFilter));
    m_minFilter = minFilter;
    m_magFilter = magFilter;
}

void Texture::setAnisotropicFiltering(float level) {
    if (level > 1.0f) {
        glBindTexture(GL_TEXTURE_2D, m_id);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, level);
        m_anisotropicLevel = level;
    }
}

void Texture::generateMipmaps() {
    glBindTexture(GL_TEXTURE_2D, m_id);
    glGenerateMipmap(GL_TEXTURE_2D);
}

int Texture::getGLInternalFormat(Format format) const {
    switch (format) {
        case Format::RGB8: return GL_RGB8;
        case Format::RGBA8: return GL_RGBA8;
        case Format::RGB16F: return GL_RGB16F;
        case Format::RGBA16F: return GL_RGBA16F;
        case Format::RGB32F: return GL_RGB32F;
        case Format::RGBA32F: return GL_RGBA32F;
        case Format::R8: return GL_R8;
        case Format::R16F: return GL_R16F;
        case Format::R32F: return GL_R32F;
        case Format::DEPTH: return GL_DEPTH_COMPONENT;
        case Format::DEPTH_STENCIL: return GL_DEPTH_STENCIL;
        default: return GL_RGBA8;
    }
}

int Texture::getGLFormat(Format format) const {
    switch (format) {
        case Format::RGB8: return GL_RGB;
        case Format::RGBA8: return GL_RGBA;
        case Format::RGB16F: return GL_RGB;
        case Format::RGBA16F: return GL_RGBA;
        case Format::RGB32F: return GL_RGB;
        case Format::RGBA32F: return GL_RGBA;
        case Format::R8: return GL_RED;
        case Format::R16F: return GL_RED;
        case Format::R32F: return GL_RED;
        case Format::DEPTH: return GL_DEPTH_COMPONENT;
        case Format::DEPTH_STENCIL: return GL_DEPTH_STENCIL;
        default: return GL_RGBA;
    }
}

int Texture::getGLType(Format format) const {
    switch (format) {
        case Format::R16F:
        case Format::R32F:
        case Format::RGB16F:
        case Format::RGBA16F:
        case Format::RGB32F:
        case Format::RGBA32F: return GL_FLOAT;
        case Format::DEPTH:
        case Format::DEPTH_STENCIL: return GL_UNSIGNED_SHORT;
        default: return GL_UNSIGNED_BYTE;
    }
}

std::vector<unsigned char> Texture::loadImageData(const std::string& filename, int& width, int& height, int& channels) {
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &channels, 0);
    if (!data) return {};

    std::vector<unsigned char> result(data, data + width * height * channels);
    stbi_image_free(data);
    return result;
}

} // namespace ygl
