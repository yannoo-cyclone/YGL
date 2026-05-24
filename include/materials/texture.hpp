#pragma once
#include <string>
#include <vector>

namespace ygl {

class Texture {
public:
    enum class Type {
        DIFFUSE, NORMAL, SPECULAR, EMISSIVE, METALLIC, ROUGHNESS,
        AMBIENT_OCCLUSION, HEIGHT, CUBEMAP, GENERIC
    };
    enum class Format {
        RGB8, RGBA8, RGB16F, RGBA16F, RGB32F, RGBA32F,
        R8, R16F, R32F, DEPTH, DEPTH_STENCIL
    };
    enum class WrapMode { REPEAT, MIRRORED_REPEAT, CLAMP_TO_EDGE, CLAMP_TO_BORDER };
    enum class FilterMode {
        NEAREST, LINEAR, NEAREST_MIPMAP_NEAREST, LINEAR_MIPMAP_NEAREST,
        NEAREST_MIPMAP_LINEAR, LINEAR_MIPMAP_LINEAR
    };

    Texture(Type type = Type::GENERIC, const std::string& name = "");
    ~Texture();

    bool loadFromFile(const std::string& filename);
    bool create(int width, int height, Format format = Format::RGBA8);
    bool create(int width, int height, Format format, const void* data);

    void bind(int textureUnit = 0) const;
    static void unbind(int textureUnit = 0);

    unsigned int getId() const { return m_id; }
    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }
    Format getFormat() const { return m_format; }
    Type getType() const { return m_type; }
    const std::string& getName() const { return m_name; }
    const std::string& getFilename() const { return m_filename; }

    void setName(const std::string& name) { m_name = name; }
    void setType(Type type) { m_type = type; }

    void setWrapMode(WrapMode mode);
    void setWrapMode(WrapMode s, WrapMode t);
    void setFilterMode(FilterMode minFilter, FilterMode magFilter);
    void setAnisotropicFiltering(float level);
    void generateMipmaps();

private:
    unsigned int m_id;
    int m_width;
    int m_height;
    Format m_format;
    Type m_type;
    std::string m_name;
    std::string m_filename;
    WrapMode m_wrapS;
    WrapMode m_wrapT;
    FilterMode m_minFilter;
    FilterMode m_magFilter;
    float m_anisotropicLevel;

    int getGLInternalFormat(Format format) const;
    int getGLFormat(Format format) const;
    int getGLType(Format format) const;
    std::vector<unsigned char> loadImageData(const std::string& filename, int& width, int& height, int& channels);
};
} // namespace ygl
