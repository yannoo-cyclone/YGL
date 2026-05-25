#include "scene/light.hpp"
#include "math/Vec3.hpp"
#include "math/Vec4.hpp"
#include "math/math_utils.hpp"

namespace ygl {

// --- Light Base Class ---
Light::Light(LightType type, const vec3& color, float intensity)
    : type(type), color(color), intensity(intensity),
      name(""), enabled(true), cast_shadows(false) {
}

Light::Light(LightType type, const vec3& color, float intensity, const std::string& name)
    : type(type), color(color), intensity(intensity),
      name(name), enabled(true), cast_shadows(false) {
}

Light::~Light() {
}

// --- Accessors ---
LightType Light::GetType() const {
    return type;
}

const vec3& Light::GetColor() const {
    return color;
}

float Light::GetIntensity() const {
    return intensity;
}

const std::string& Light::GetName() const {
    return name;
}

bool Light::IsEnabled() const {
    return enabled;
}

bool Light::CastShadows() const {
    return cast_shadows;
}

void Light::SetType(LightType t) {
    type = t;
}

void Light::SetColor(const vec3& c) {
    color = c;
}

void Light::SetIntensity(float i) {
    intensity = i;
}

void Light::SetName(const std::string& n) {
    name = n;
}

void Light::SetEnabled(bool e) {
    enabled = e;
}

void Light::SetCastShadows(bool s) {
    cast_shadows = s;
}

// --- Virtual Methods ---
vec3 Light::GetDirection(const vec3& point) const {
    return normalize(point - GetPosition());
}

vec3 Light::GetPosition() const {
    return vec3(0.0f, 0.0f, 0.0f);
}

float Light::GetAttenuation(float distance) const {
    return 1.0f;
}

float Light::GetRange() const {
    return 0.0f;
}

vec3 Light::GetRadiance(const vec3& point, const vec3& normal) const {
    if (!enabled) return vec3(0.0f);

    vec3 direction = GetDirection(point);
    float NdotL = std::max(dot(normal, direction), 0.0f);

    vec3 radiance = color * intensity * NdotL;

    float distance = length(point - GetPosition());
    float attenuation = GetAttenuation(distance);

    return radiance * attenuation;
}

void Light::Update(float delta_time) {
}

void Light::SetScene(Scene* s) {
    scene = s;
}

Scene* Light::GetScene() const {
    return scene;
}

// --- Directional Light ---
DirectionalLight::DirectionalLight(const vec3& direction, const vec3& color, float intensity)
    : Light(LightType::DIRECTIONAL, color, intensity),
      direction(normalize(direction)) {
}

DirectionalLight::DirectionalLight(const vec3& direction, const vec3& color, float intensity, const std::string& name)
    : Light(LightType::DIRECTIONAL, color, intensity, name),
      direction(normalize(direction)) {
}

vec3 DirectionalLight::GetDirection(const vec3& point) const {
    return -direction;
}

void DirectionalLight::SetDirection(const vec3& d) {
    direction = normalize(d);
}

const vec3& DirectionalLight::GetDirectionVector() const {
    return direction;
}

// --- Point Light ---
PointLight::PointLight(const vec3& position, const vec3& color, float intensity)
    : Light(LightType::POINT, color, intensity),
      position(position),
      constant_attenuation(1.0f),
      linear_attenuation(0.0f),
      quadratic_attenuation(0.0f),
      range(0.0f) {
}

PointLight::PointLight(const vec3& position, const vec3& color, float intensity, const std::string& name)
    : Light(LightType::POINT, color, intensity, name),
      position(position),
      constant_attenuation(1.0f),
      linear_attenuation(0.0f),
      quadratic_attenuation(0.0f),
      range(0.0f) {
}

vec3 PointLight::GetPosition() const {
    return position;
}

void PointLight::SetPosition(const vec3& p) {
    position = p;
}

vec3 PointLight::GetDirection(const vec3& point) const {
    return normalize(point - position);
}

float PointLight::GetAttenuation(float distance) const {
    if (range > 0.0f && distance > range) {
        return 0.0f;
    }
    return 1.0f / (constant_attenuation +
                  linear_attenuation * distance +
                  quadratic_attenuation * distance * distance);
}

float PointLight::GetRange() const {
    return range;
}

void PointLight::SetAttenuation(float constant, float linear, float quadratic) {
    constant_attenuation = constant;
    linear_attenuation = linear;
    quadratic_attenuation = quadratic;
}

void PointLight::SetRange(float r) {
    range = r;
}

// --- Spot Light ---
SpotLight::SpotLight(const vec3& position, const vec3& direction,
                     const vec3& color, float intensity,
                     float inner_angle, float outer_angle)
    : Light(LightType::SPOT, color, intensity),
      position(position),
      direction(normalize(direction)),
      inner_angle(inner_angle),
      outer_angle(outer_angle),
      constant_attenuation(1.0f),
      linear_attenuation(0.0f),
      quadratic_attenuation(0.0f),
      range(0.0f) {
    inner_cos = cos(inner_angle * DEG_TO_RAD);
    outer_cos = cos(outer_angle * DEG_TO_RAD);
}

SpotLight::SpotLight(const vec3& position, const vec3& direction,
                     const vec3& color, float intensity,
                     float inner_angle, float outer_angle, const std::string& name)
    : Light(LightType::SPOT, color, intensity, name),
      position(position),
      direction(normalize(direction)),
      inner_angle(inner_angle),
      outer_angle(outer_angle),
      constant_attenuation(1.0f),
      linear_attenuation(0.0f),
      quadratic_attenuation(0.0f),
      range(0.0f) {
    inner_cos = cos(inner_angle * DEG_TO_RAD);
    outer_cos = cos(outer_angle * DEG_TO_RAD);
}

vec3 SpotLight::GetPosition() const {
    return position;
}

void SpotLight::SetPosition(const vec3& p) {
    position = p;
}

vec3 SpotLight::GetDirection(const vec3& point) const {
    return normalize(point - position);
}

const vec3& SpotLight::GetDirectionVector() const {
    return direction;
}

void SpotLight::SetDirection(const vec3& d) {
    direction = normalize(d);
    inner_cos = cos(inner_angle * DEG_TO_RAD);
    outer_cos = cos(outer_angle * DEG_TO_RAD);
}

void SpotLight::SetAngles(float inner, float outer) {
    inner_angle = inner;
    outer_angle = outer;
    inner_cos = cos(inner * DEG_TO_RAD);
    outer_cos = cos(outer * DEG_TO_RAD);
}

float SpotLight::GetInnerAngle() const {
    return inner_angle;
}

float SpotLight::GetOuterAngle() const {
    return outer_angle;
}

float SpotLight::GetAttenuation(float distance) const {
    if (range > 0.0f && distance > range) {
        return 0.0f;
    }
    return 1.0f / (constant_attenuation +
                  linear_attenuation * distance +
                  quadratic_attenuation * distance * distance);
}

float SpotLight::GetRange() const {
    return range;
}

void SpotLight::SetAttenuation(float constant, float linear, float quadratic) {
    constant_attenuation = constant;
    linear_attenuation = linear;
    quadratic_attenuation = quadratic;
}

void SpotLight::SetRange(float r) {
    range = r;
}

vec3 SpotLight::GetRadiance(const vec3& point, const vec3& normal) const {
    if (!enabled) return vec3(0.0f);

    vec3 light_dir = normalize(position - point);
    float distance = length(position - point);

    float cos_theta = dot(light_dir, direction);
    if (cos_theta < outer_cos) {
        return vec3(0.0f);
    }

    float falloff = 1.0f;
    if (cos_theta < inner_cos) {
        falloff = (cos_theta - outer_cos) / (inner_cos - outer_cos);
        falloff = clamp(falloff, 0.0f, 1.0f);
    }

    float NdotL = std::max(dot(normal, light_dir), 0.0f);
    vec3 radiance = color * intensity * NdotL * falloff;

    float attenuation = GetAttenuation(distance);
    return radiance * attenuation;
}

// --- Area Light (Rectangle) ---
AreaLight::AreaLight(const vec3& position, const vec3& u, const vec3& v,
                    const vec3& color, float intensity)
    : Light(LightType::AREA, color, intensity),
      position(position),
      u_axis(normalize(u)),
      v_axis(normalize(v)),
      normal(normalize(cross(u, v))) {
    area = length(u) * length(v);
}

AreaLight::AreaLight(const vec3& position, const vec3& u, const vec3& v,
                    const vec3& color, float intensity, const std::string& name)
    : Light(LightType::AREA, color, intensity, name),
      position(position),
      u_axis(normalize(u)),
      v_axis(normalize(v)),
      normal(normalize(cross(u, v))) {
    area = length(u) * length(v);
}

vec3 AreaLight::GetPosition() const {
    return position;
}

const vec3& AreaLight::GetUAxis() const {
    return u_axis;
}

const vec3& AreaLight::GetVAxis() const {
    return v_axis;
}

const vec3& AreaLight::GetNormal() const {
    return normal;
}

float AreaLight::GetArea() const {
    return area;
}

void AreaLight::SetPosition(const vec3& p) {
    position = p;
}

void AreaLight::SetAxes(const vec3& u, const vec3& v) {
    u_axis = normalize(u);
    v_axis = normalize(v);
    normal = normalize(cross(u_axis, v_axis));
    area = length(u) * length(v);
}

vec3 AreaLight::GetRadiance(const vec3& point, const vec3& normal) const {
    if (!enabled) return vec3(0.0f);

    vec3 to_light = position - point;
    float distance = length(to_light);
    vec3 light_dir = to_light / distance;

    float NdotL = std::max(dot(normal, light_dir), 0.0f);
    if (NdotL <= 0.0f) return vec3(0.0f);

    float attenuation = 1.0f / (distance * distance);
    vec3 radiance = color * intensity * NdotL * attenuation * area;

    return radiance;
}

// --- Light Factory Functions ---
DirectionalLight* CreateDirectionalLight(const vec3& direction, const vec3& color, float intensity, const std::string& name) {
    return new DirectionalLight(direction, color, intensity, name);
}

PointLight* CreatePointLight(const vec3& position, const vec3& color, float intensity, const std::string& name) {
    return new PointLight(position, color, intensity, name);
}

SpotLight* CreateSpotLight(const vec3& position, const vec3& direction, const vec3& color,
                           float intensity, float inner_angle, float outer_angle, const std::string& name) {
    return new SpotLight(position, direction, color, intensity, inner_angle, outer_angle, name);
}

AreaLight* CreateAreaLight(const vec3& position, const vec3& u, const vec3& v,
                           const vec3& color, float intensity, const std::string& name) {
    return new AreaLight(position, u, v, color, intensity, name);
}

} // namespace ygl
