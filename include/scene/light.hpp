#pragma once

#include "core/object3d.hpp"
#include "math/vec3.hpp"
#include "math/vec4.hpp"

namespace ygl {

class Light : public Object3D {
public:
    enum class Type {
        DIRECTIONAL,
        POINT,
        SPOT,
        AREA,
        ENVIRONMENT
    };

    Light(Type type = Type::POINT, const std::string& name = "Light");
    virtual ~Light();

    // Type
    void setType(Type type) { m_type = type; }
    Type getType() const { return m_type; }

    // Color
    void setColor(const Vec3& color) { m_color = color; }
    const Vec3& getColor() const { return m_color; }

    // Intensity
    void setIntensity(float intensity) { m_intensity = intensity; }
    float getIntensity() const { return m_intensity; }

    // Shadow
    void setCastShadows(bool castShadows) { m_castShadows = castShadows; }
    bool isCastingShadows() const { return m_castShadows; }

    // Range (for point and spot lights)
    void setRange(float range) { m_range = range; }
    float getRange() const { return m_range; }

    // Attenuation (for point and spot lights)
    void setAttenuation(const Vec3& attenuation) { m_attenuation = attenuation; }
    const Vec3& getAttenuation() const { return m_attenuation; }

    // Spot light properties
    void setInnerAngle(float angle) { m_innerAngle = angle; }
    float getInnerAngle() const { return m_innerAngle; }

    void setOuterAngle(float angle) { m_outerAngle = angle; }
    float getOuterAngle() const { return m_outerAngle; }

    void setSpotDirection(const Vec3& direction) { m_spotDirection = direction; }
    const Vec3& getSpotDirection() const { return m_spotDirection; }

    // Area light properties
    void setAreaSize(const Vec3& size) { m_areaSize = size; }
    const Vec3& getAreaSize() const { return m_areaSize; }

    // Environment light properties
    void setEnvironmentMap(uint32_t textureId) { m_environmentMap = textureId; }
    uint32_t getEnvironmentMap() const { return m_environmentMap; }

    // Bind for rendering
    virtual void bind() const;
    virtual void unbind() const;

private:
    Type m_type;
    Vec3 m_color = Vec3(1.0f);
    float m_intensity = 1.0f;

    bool m_castShadows = true;

    // Range and attenuation
    float m_range = 100.0f;
    Vec3 m_attenuation = Vec3(1.0f, 0.01f, 0.0001f);

    // Spot light
    float m_innerAngle = 20.0f;
    float m_outerAngle = 45.0f;
    Vec3 m_spotDirection = Vec3(0.0f, -1.0f, 0.0f);

    // Area light
    Vec3 m_areaSize = Vec3(1.0f);

    // Environment light
    uint32_t m_environmentMap = 0;
};

} // namespace ygl
