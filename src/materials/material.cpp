#include "materials/material.hpp"
#include "math/vec3.hpp"
#include "math/vec4.hpp"
#include <algorithm>

namespace ygl {

Material::Material()
    : albedo(1.0f), roughness(0.5f), metallic(0.0f), emission(0.0f), ior(1.5f),
      emission_strength(1.0f), has_albedo_texture(false), has_normal_texture(false),
      has_roughness_texture(false), has_metallic_texture(false), has_emission_texture(false),
      albedo_texture_id(0), normal_texture_id(0), roughness_texture_id(0),
      metallic_texture_id(0), emission_texture_id(0), type(MaterialType::PBR_METALLIC_ROUGHNESS) {}

Material::Material(const vec4& color)
    : Material() { albedo = color; }

Material::~Material() {}

// --- Setters ---
void Material::SetAlbedo(const vec4& color) { albedo = color; }
void Material::SetRoughness(float r) { roughness = std::clamp(r, 0.0f, 1.0f); }
void Material::SetMetallic(float m) { metallic = std::clamp(m, 0.0f, 1.0f); }
void Material::SetEmission(const vec3& e, float strength) { emission = e; emission_strength = strength; }
void Material::SetIOR(float index) { ior = index; }
void Material::SetAlbedoTexture(unsigned int id) { albedo_texture_id = id; has_albedo_texture = true; }
void Material::SetNormalTexture(unsigned int id) { normal_texture_id = id; has_normal_texture = true; }
void Material::SetRoughnessTexture(unsigned int id) { roughness_texture_id = id; has_roughness_texture = true; }
void Material::SetMetallicTexture(unsigned int id) { metallic_texture_id = id; has_metallic_texture = true; }
void Material::SetEmissionTexture(unsigned int id) { emission_texture_id = id; has_emission_texture = true; }
void Material::SetType(MaterialType t) { type = t; }

// --- Getters ---
vec4 Material::GetAlbedo() const { return albedo; }
float Material::GetRoughness() const { return roughness; }
float Material::GetMetallic() const { return metallic; }
vec3 Material::GetEmission() const { return emission * emission_strength; }
float Material::GetIOR() const { return ior; }
unsigned int Material::GetAlbedoTexture() const { return albedo_texture_id; }
unsigned int Material::GetNormalTexture() const { return normal_texture_id; }
unsigned int Material::GetRoughnessTexture() const { return roughness_texture_id; }
unsigned int Material::GetMetallicTexture() const { return metallic_texture_id; }
unsigned int Material::GetEmissionTexture() const { return emission_texture_id; }
MaterialType Material::GetType() const { return type; }
bool Material::HasAlbedoTexture() const { return has_albedo_texture; }
bool Material::HasNormalTexture() const { return has_normal_texture; }
bool Material::HasRoughnessTexture() const { return has_roughness_texture; }
bool Material::HasMetallicTexture() const { return has_metallic_texture; }
bool Material::HasEmissionTexture() const { return has_emission_texture; }

// --- Sampling ---
vec3 Material::Sample(const vec3& wi, const vec3& wo, const vec3& normal, const vec2& texcoord, const vec3& position) const {
    return vec3(albedo) * (1.0f / M_PI); // Simplified PBR
}

float Material::PDF(const vec3& wi, const vec3& wo, const vec3& normal) const {
    return 1.0f / (2.0f * M_PI);
}

} // namespace ygl
