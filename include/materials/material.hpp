#pragma once

#include <algorithm>
#include <memory>

namespace ygl {

class Material {
public:
    Material() : m_roughness(0.5f), m_metallic(0.5f), m_opacity(1.0f) {}

    // Getters
    float getRoughness() const { return m_roughness; }
    float getMetallic() const { return m_metallic; }
    float getOpacity() const { return m_opacity; }

    // Setters
    void setRoughness(float roughness) { m_roughness = std::clamp(roughness, 0.0f, 1.0f); }
    void setMetallic(float metallic) { m_metallic = std::clamp(metallic, 0.0f, 1.0f); }
    void setOpacity(float opacity) { m_opacity = std::clamp(opacity, 0.0f, 1.0f); }

    // Méthodes virtuelles pour bind/unbind
    virtual void bind() const {}
    virtual void unbind() const {}

private:
    float m_roughness;
    float m_metallic;
    float m_opacity;
};

} // namespace ygl
