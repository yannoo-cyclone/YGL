#include "materials/texture.hpp"
#include "stb_image.hpp"
#include <GL/glew.h>
#include <algorithm>
#include <vector>

namespace ygl {

Texture::Texture(Type type, const std::string& name)
    : m_type(type),
      m_name(name),
      m_id(0),
      m_width(0),
      m_height(0),
      m_format(Format::RGBA8),
      m_wrapS(WrapMode::REPEAT),
      m_wrapT(WrapMode::REPEAT),
      m_minFilter(FilterMode::LINEAR),
      m_magFilter(FilterMode::LINEAR),
      m_anisotropicLevel(1.0f) {
    glGenTextures(1, &m_id);
}

Texture::~Texture() {
    if (m_id != 0) {
        glDeleteTextures(1, &m_id);
    }
}

bool Texture::loadFromFile(const std::string& filename) {
    int w, h, c;
    stbi_set_flip_vertically_on_load(true);
    stbi_uc* data = stbi_load(filename.c_str(), &w, &h, &c, 0);
    if (!data) {
        return false;
    }

    m_width = w;
    m_height = h;
    m_filename = filename;

    GLenum gl_format;
    if (c == 1) {
        m_format = Format::R8;
        gl_format = GL_RED;
    } else if (c == 2) {
        m_format = Format::RGBA8;
        gl_format = GL_RG;
    } else if (c == 3) {
        m_format = Format::RGB8;
        gl_format = GL_RGB;
    } else {
        m_format = Format::RGBA8;
        gl_format = GL_RGBA;
    }

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
    glTexParameteri(GL_TEXTURE_2
