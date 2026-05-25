#pragma once
#include "math/Vec3.hpp"
#include "math/Vec4.hpp"
#include "math/Vec2.hpp"
#include <algorithm>
#include <memory>

namespace ygl {
enum class MaterialType {
    PBR_METALLIC_ROUGHNESS,
    PBR_SPECULAR_GLOSSINESS,
    PHONG,
    FLAT
};

class Material {
public:
    Material();
    Material(const Vec4& color);
    ~Material();

    // Setters
    void SetAlbedo(const Vec4& color);
    void SetRoughness(float roughness);
    void SetMetallic(float metallic);
    void SetEmission(const Vec3& emission, float strength);
    void SetIOR(float index);
    void SetAlbedoTexture(unsigned int id);
    void SetNormalTexture(unsigned int id);
    void SetRoughnessTexture(unsigned int id);
    void SetMetallicTexture(unsigned int id);
    void SetEmissionTexture(unsigned int id);
    void SetType(MaterialType type);

    // Getters
    Vec4 GetAlbedo() const;
    float GetRoughness() const;
    float GetMetallic() const;
    Vec3 GetEmission() const;
    float GetIOR() const;
    unsigned int GetAlbedoTexture() const;
    unsigned int GetNormalTexture() const;
    unsigned int GetRoughnessTexture() const;
    unsigned int GetMetallicTexture() const;
    unsigned int GetEmissionTexture() const;
    MaterialType GetType() const;
    bool HasAlbedoTexture() const;
    bool HasNormalTexture() const;
    bool HasRoughnessTexture() const;
    bool HasMetallicTexture() const;
    bool HasEmissionTexture() const;

    // Sampling
    Vec3 Sample(const Vec3& wi, const Vec3& wo, const Vec3& normal, const Vec2& texcoord, const Vec3& position) const;
    float PDF(const Vec3& wi, const Vec3& wo, const Vec3& normal) const;

    // Bind/Unbind
    virtual void bind() const;
    virtual void unbind() const;

private:
    Vec4 albedo;
    float roughness;
    float metallic;
    Vec3 emission;
    float ior;
    float emission_strength;
    bool has_albedo_texture;
    bool has_normal_texture;
    bool has_roughness_texture;
    bool has_metallic_texture;
    bool has_emission_texture;
    unsigned int albedo_texture_id;
    unsigned int normal_texture_id;
    unsigned int roughness_texture_id;
    unsigned int metallic_texture_id;
    unsigned int emission_texture_id;
    MaterialType type;
};
} // namespace ygl
