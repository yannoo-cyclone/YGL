#pragma once

#include "math/vec3.hpp"
#include "math/vec4.hpp"
#include <string>
#include <memory>

namespace ygl {

class Texture;

class Material {
public:
    enum class Type {
        OPAQUE,
        TRANSPARENT,
        EMISSIVE,
        REFLECTIVE,
        REFRACTIVE
    };

    enum class BlendMode {
        OPAQUE,
        ALPHA_BLEND,
        ADDITIVE,
        MULTIPLY
    };

    Material(const std::string& name = "");
    virtual ~Material();

    const std::string& getName() const { return m_name; }
    void setName(const std::string& name) { m_name = name; }

    // Base properties
    void setAlbedo(const Vec3& albedo) { m_albedo = albedo; }
    const Vec3& getAlbedo() const { return m_albedo; }
    void setAlbedo(const Vec4& albedo) { m_albedo = Vec3(albedo.x, albedo.y, albedo.z); m_opacity = albedo.w; }
    Vec4 getAlbedoRGBA() const { return Vec4(m_albedo, m_opacity); }

    // PBR properties
    void setRoughness(float roughness) { m_roughness = std::clamp(roughness, 0.0f, 1.0f); }
    float getRoughness() const { return m_roughness; }

    void setMetallic(float metallic) { m_metallic = std::clamp(metallic, 0.0f, 1.0f); }
    float getMetallic() const { return m_metallic; }

    void setEmission(const Vec3& emission) { m_emission = emission; }
    const Vec3& getEmission() const { return m_emission; }

    void setEmissionIntensity(float intensity) { m_emissionIntensity = intensity; }
    float getEmissionIntensity() const { return m_emissionIntensity; }

    void setIOR(float ior) { m_ior = ior; }
    float getIOR() const { return m_ior; }

    // Opacity and transparency
    void setOpacity(float opacity) { m_opacity = std::clamp(opacity, 0.0f, 1.0f); }
    float getOpacity() const { return m_opacity; }

    void setBlendMode(BlendMode mode) { m_blendMode = mode; }
    BlendMode getBlendMode() const { return m_blendMode; }

    // Normal mapping
    void setNormalScale(float scale) { m_normalScale = scale; }
    float getNormalScale() const { return m_normalScale; }

    // Textures
    void setAlbedoMap(std::shared_ptr<Texture> texture) { m_albedoMap = texture; }
    std::shared_ptr<Texture> getAlbedoMap() const { return m_albedoMap; }

    void setNormalMap(std::shared_ptr<Texture> texture) { m_normalMap = texture; }
    std::shared_ptr<Texture> getNormalMap() const { return m_normalMap; }

    void setMetallicMap(std::shared_ptr<Texture> texture) { m_metallicMap = texture; }
    std::shared_ptr<Texture> getMetallicMap() const { return m_metallicMap; }

    void setRoughnessMap(std::shared_ptr<Texture> texture) { m_roughnessMap = texture; }
    std::shared_ptr<Texture> getRoughnessMap() const { return m_roughnessMap; }

    void setEmissionMap(std::shared_ptr<Texture> texture) { m_emissionMap = texture; }
    std::shared_ptr<Texture> getEmissionMap() const { return m_emissionMap; }

    void setOpacityMap(std::shared_ptr<Texture> texture) { m_opacityMap = texture; }
    std::shared_ptr<Texture> getOpacityMap() const { return m_opacityMap; }

    // Type
    void setType(Type type) { m_type = type; }
    Type getType() const { return m_type; }

    // Two-sided rendering
    void setTwoSided(bool twoSided) { m_twoSided = twoSided; }
    bool isTwoSided() const { return m_twoSided; }

    // Wireframe rendering
    void setWireframe(bool wireframe) { m_wireframe = wireframe; }
    bool isWireframe() const { return m_wireframe; }

    // Bind for rendering
    virtual void bind() const;
    virtual void unbind() const;

protected:
    std::string m_name;

    Vec3 m_albedo = Vec3(1.0f);
    float m_opacity = 1.0f;

    float m_roughness = 0.5f;
    float m_metallic = 0.0f;
    Vec3 m_emission = Vec3(0.0f);
    float m_emissionIntensity = 1.0f;
    float m_ior = 1.5f;
    float m_normalScale = 1.0f;

    Type m_type = Type::OPAQUE;
    BlendMode m_blendMode = BlendMode::OPAQUE;
    bool m_twoSided = false;
    bool m_wireframe = false;

    std::shared_ptr<Texture> m_albedoMap;
    std::shared_ptr<Texture> m_normalMap;
    std::shared_ptr<Texture> m_metallicMap;
    std::shared_ptr<Texture> m_roughnessMap;
    std::shared_ptr<Texture> m_emissionMap;
    std::shared_ptr<Texture> m_opacityMap;
};

} // namespace ygl
