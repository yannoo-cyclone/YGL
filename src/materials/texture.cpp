#include "materials/texture.hpp"
#include <stb_image.hpp>
#include <algorithm>

namespace ygl {

// --- Texture2D ---
Texture::Texture()
    : id(0), width(0), height(0), channels(0),
      internal_format(GL_RGBA), format(GL_RGBA), type(GL_UNSIGNED_BYTE), wrapped(false) {
    glGenTextures(1, &id);
}

Texture::Texture(const std::string& filepath, bool flip)
    : Texture() { LoadFromFile(filepath, flip); }

Texture::~Texture() {
    if (id != 0) glDeleteTextures(1, &id);
}

bool Texture::LoadFromFile(const std::string& filepath, bool flip) {
    int w, h, c;
    stbi_set_flip_vertically_on_load(flip);
    unsigned char* data = stbi_load(filepath.c_str(), &w, &h, &c, 0);
    if (!data) return false;

    width = w; height = h; channels = c;
    if (channels == 1) { internal_format = GL_R8; format = GL_RED; }
    else if (channels == 2) { internal_format = GL_RG8; format = GL_RG; }
    else if (channels == 3) { internal_format = GL_RGB8; format = GL_RGB; }
    else if (channels == 4) { internal_format = GL_RGBA8; format = GL_RGBA; }

    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
    return true;
}

bool Texture::LoadFromMemory(int w, int h, int c, const unsigned char* data) {
    width = w; height = h; channels = c;
    if (channels == 1) { internal_format = GL_R8; format = GL_RED; }
    else if (channels == 2) { internal_format = GL_RG8; format = GL_RG; }
    else if (channels == 3) { internal_format = GL_RGB8; format = GL_RGB; }
    else if (channels == 4) { internal_format = GL_RGBA8; format = GL_RGBA; }

    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    return true;
}

void Texture::Bind(GLenum unit) const { glActiveTexture(unit); glBindTexture(GL_TEXTURE_2D, id); }
void Texture::Unbind() const { glBindTexture(GL_TEXTURE_2D, 0); }
void Texture::SetWrapMode(GLenum s, GLenum t) { glBindTexture(GL_TEXTURE_2D, id); glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, s); glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, t); wrapped = true; }
void Texture::SetFilterMode(GLenum min, GLenum mag) { glBindTexture(GL_TEXTURE_2D, id); glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min); glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag); }
void Texture::GenerateMipmaps() { glBindTexture(GL_TEXTURE_2D, id); glGenerateMipmap(GL_TEXTURE_2D); }
unsigned int Texture::GetID() const { return id; }
int Texture::GetWidth() const { return width; }
int Texture::GetHeight() const { return height; }
int Texture::GetChannels() const { return channels; }
GLenum Texture::GetInternalFormat() const { return internal_format; }
GLenum Texture::GetFormat() const { return format; }
GLenum Texture::GetType() const { return type; }
vec4 Texture::Sample(const vec2& tc) const { return vec4(1.0f); } // Placeholder

// --- TextureCubeMap ---
TextureCubeMap::TextureCubeMap() : Texture() {
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);
    for (unsigned int i = 0; i < 6; ++i)
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

TextureCubeMap::TextureCubeMap(const std::vector<std::string>& faces) : Texture() { LoadFromFiles(faces); }

bool TextureCubeMap::LoadFromFiles(const std::vector<std::string>& faces) {
    if (faces.size() != 6) return false;
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);
    for (unsigned int i = 0; i < 6; ++i) {
        int w, h, c;
        unsigned char* data = stbi_load(faces[i].c_str(), &w, &h, &c, 0);
        if (!data) return false;
        if (i == 0) { width = w; height = h; channels = c; }
        GLenum fmt = c == 3 ? GL_RGB : GL_RGBA;
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, fmt, w, h, 0, fmt, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    return true;
}

void TextureCubeMap::Bind(GLenum unit) const { glActiveTexture(unit); glBindTexture(GL_TEXTURE_CUBE_MAP, id); }

} // namespace ygl
