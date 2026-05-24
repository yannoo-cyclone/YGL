#include <GL/glew.h>
#include <GL/gl.h>
#include "materials/texture.hpp"
#include <stb_image.hpp>
#include <algorithm>
#include <iostream>

namespace ygl {

// --- Texture Constructors ---
Texture::Texture(Type type, const std::string& name)
    : m_id(0),
      m_width(0),
      m_height(0),
      m_format(Format::RGBA8),
      m_type(type),
      m_name(name),
      m_filename("") {
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

    GLenum internal_format, format;
    if (c == 1) { internal_format = GL_R8; format = GL_RED; }
    else if (c == 2) { internal_format = GL_RG8; format = GL_RG; }
    else if (c == 3) { internal_format = GL_RGB8; format = GL_RGB; }
    else if (c == 4) { internal_format = GL_RGBA8; format = GL_RGBA; }
    else { stbi_image_free(data); return false; }

    glBindTexture(GL_TEXTURE_2D, m_id);
    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
    return true;
}

bool Texture::create(int width, int height, Format format) {
    m_width = width;
    m_height = height;
    m_format = format;

    GLenum gl_format, gl_internal_format;
    switch (format) {
        case Format::RGB8: gl_internal_format = GL_RGB8; gl_format = GL_RGB; break;
        case Format::RGBA8: gl_internal_format = GL_RGBA8; gl_format = GL_RGBA; break;
        case Format::RGB16F: gl_internal_format = GL_RGB16F; gl_format = GL_RGB; break;
        case Format::RGBA16F: gl_internal_format = GL_RGBA16F; gl_format = GL_RGBA; break;
        case Format::RGB32F: gl_internal_format = GL_RGB32F; gl_format = GL_RGB; break;
        case Format::RGBA32F: gl_internal_format = GL_RGBA32F; gl_format = GL_RGBA; break;
        case Format::R8: gl_internal_format = GL_R8; gl_format = GL_RED; break;
        case Format::R16F: gl_internal_format = GL_R16F; gl_format = GL_RED; break;
        case Format::R32F: gl_internal_format = GL_R32F; gl_format = GL_RED; break;
        case Format::DEPTH: gl_internal_format = GL_DEPTH_COMPONENT; gl_format = GL_DEPTH_COMPONENT; break;
        case Format::DEPTH_STENCIL: gl_internal_format = GL_DEPTH_STENCIL; gl_format = GL_DEPTH_STENCIL; break;
        default: return false;
    }

    glBindTexture(GL_TEXTURE_2D, m_id);
    glTexImage2D(GL_TEXTURE_2D, 0, gl_internal_format, m_width, m_height, 0, gl_format, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    return true;
}

bool Texture::create(int width, int height, Format format, const void* data) {
    if (!create(width, height, format)) return false;
    glBindTexture(GL_TEXTURE_2D, m_id);
    GLenum gl_format = getGLFormat(format);
    GLenum gl_type = getGLType(format);
    glTexImage2D(GL_TEXTURE_2D, 0, getGLInternalFormat(format), m_width, m_height, 0, gl_format, gl_type, data);
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
    m_wrapS = s;
    m_wrapT = t;
    glBindTexture(GL_TEXTURE_2D, m_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(s));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(t));
}

void Texture::setFilterMode(FilterMode minFilter, FilterMode magFilter) {
    m_minFilter = minFilter;
    m_magFilter = magFilter;
    glBindTexture(GL_TEXTURE_2D, m_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(minFilter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(magFilter));
}

void Texture::setAnisotropicFiltering(float level) {
    m_anisotropicLevel = level;
    glBindTexture(GL_TEXTURE_2D, m_id);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, level);
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
        case Format::RGB8: return GL_UNSIGNED_BYTE;
        case Format::RGBA8: return GL_UNSIGNED_BYTE;
        case Format::RGB16F: return GL_FLOAT;
        case Format::RGBA16F: return GL_FLOAT;
        case Format::RGB32F: return GL_FLOAT;
        case Format::RGBA32F: return GL_FLOAT;
        case Format::R8: return GL_UNSIGNED_BYTE;
        case Format::R16F: return GL_FLOAT;
        case Format::R32F: return GL_FLOAT;
        case Format::DEPTH: return GL_FLOAT;
        case Format::DEPTH_STENCIL: return GL_UNSIGNED_INT_24_8;
        default: return GL_UNSIGNED_BYTE;
    }
}

std::vector<unsigned char> Texture::loadImageData(const std::string& filename, int& width, int& height, int& channels) {
    stbi_set_flip_vertically_on_load(true);
    int w, h, c;
    unsigned char* data = stbi_load(filename.c_str(), &w, &h, &c, 0);
    if (!data) return {};
    width = w; height = h; channels = c;
    std::vector<unsigned char> result(data, data + w * h * c);
    stbi_image_free(data);
    return result;
}

} // namespace ygl
