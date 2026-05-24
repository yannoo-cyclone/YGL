#include "materials/material.hpp"
#include <cmath>

namespace ygl {
Material::Material()
    : albedo(Vec4(1.0f)), roughness(0.5f), metallic(0.0f), emission(Vec3(0.0f)), ior(1.5f),
      emission_strength(1.0f), has_albedo_texture(false), has_normal_texture(false),
      has_roughness_texture(false), has_metallic_texture(false), has_emission_texture(false),
      albedo_texture_id(0), normal_texture_id(0), roughness_texture_id(0),
      metallic_texture_id(0), emission_texture_id(0), type(MaterialType::PBR_METALLIC_ROUGHNESS) {}

Material::Material(const Vec4& color) : Material() { albedo = color; }
Material::~Material() {}

void Material::SetAlbedo(const Vec4& color) { albedo = color; }
void Material::SetRoughness(float r) { roughness = std::clamp(r, 0.0f, 1.0f); }
void Material::SetMetallic(float m) { metallic = std::clamp(m, 0.0f, 1.0f); }
void Material::SetEmission(const Vec3& e, float strength) { emission = e; emission_strength = strength; }
void Material::SetIOR(float index) { ior = index; }
void Material::SetAlbedoTexture(unsigned int id) { albedo_texture_id = id; has_albedo_texture = true; }
void Material::SetNormalTexture(unsigned int id) { normal_texture_id = id; has_normal_texture = true; }
void Material::SetRoughnessTexture(unsigned int id) { roughness_texture_id = id; has_roughness_texture = true; }
void Material::SetMetallicTexture(unsigned int id) { metallic_texture_id = id; has_metallic_texture = true; }
void Material::SetEmissionTexture(unsigned int id) { emission_texture_id = id; has_emission_texture = true; }
void Material::SetType(MaterialType t) { type = t; }

Vec4 Material::GetAlbedo() const { return albedo; }
float Material::GetRoughness() const { return roughness; }
float Material::GetMetallic() const { return metallic; }
Vec3 Material::GetEmission() const { return emission * emission_strength; }
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

Vec3 Material::Sample(const Vec3& /*wi*/, const Vec3& /*wo*/, const Vec3& /*normal*/, const Vec2& /*texcoord*/, const Vec3& /*position*/) const {
    return Vec3(albedo.x, albedo.y, albedo.z) * (1.0f / M_PI);
}
float Material::PDF(const Vec3& /*wi*/, const Vec3& /*wo*/, const Vec3& /*normal*/) const {
    return 1.0f / (2.0f * M_PI);
}

void Material::bind() const {}
void Material::unbind() const {}
} // namespace ygl
