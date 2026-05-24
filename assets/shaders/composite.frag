#version 330 core

// Composite Fragment Shader
// Combines spherical G-Buffer radiosity with direct lighting and other effects

in vec2 vTexCoord;
out vec4 FragColor;

// Textures from G-Buffer
uniform sampler2D uAlbedoTexture;
uniform sampler2D uNormalTexture;
uniform sampler2D uPositionTexture;
uniform sampler2D uDepthTexture;

// Spherical G-Buffer result
uniform sampler2D uSphericalResult;

// Screen parameters
uniform vec2 uScreenSize;

// Lighting parameters
uniform vec3 uLightPosition;
uniform vec3 uLightColor;
uniform float uLightIntensity;

// Camera parameters
uniform vec3 uCameraPosition;
uniform mat4 uInverseViewProjection;

// Tone mapping parameters
uniform float uExposure;
uniform float uGamma;

// Reconstruct world position from depth
vec3 ReconstructWorldPosition(float depth, vec2 texCoord) {
    vec2 ndc = texCoord * 2.0 - 1.0;
    vec4 clipPos = vec4(ndc, depth, 1.0);
    vec4 viewPos = uInverseViewProjection * clipPos;
    viewPos.xyz /= viewPos.w;
    return viewPos.xyz;
}

// Reinhard tone mapping
vec3 ToneMapReinhard(vec3 color) {
    color *= uExposure;
    color = color / (1.0 + color);
    return pow(color, vec3(1.0 / uGamma));
}

// Calculate direct lighting
vec3 CalculateDirectLighting(vec3 worldPos, vec3 normal, vec3 albedo) {
    vec3 lightDir = normalize(uLightPosition - worldPos);
    float NdotL = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = albedo * NdotL * uLightColor * uLightIntensity;
    return diffuse;
}

// Calculate ambient lighting from spherical G-Buffer
vec3 CalculateAmbientLighting(vec3 albedo) {
    vec4 spherical = texture(uSphericalResult, vTexCoord);
    return spherical.rgb * albedo;
}

void main() {
    // Sample G-Buffer textures
    vec4 albedoTex = texture(uAlbedoTexture, vTexCoord);
    vec3 normal = normalize(texture(uNormalTexture, vTexCoord).rgb * 2.0 - 1.0);
    vec3 position = texture(uPositionTexture, vTexCoord).rgb;
    float depth = texture(uDepthTexture, vTexCoord).r;

    // Reconstruct world position
    vec3 worldPos = ReconstructWorldPosition(depth, vTexCoord);

    // Sample spherical G-Buffer result
    vec4 sphericalResult = texture(uSphericalResult, vTexCoord);

    // Calculate direct lighting
    vec3 directLighting = CalculateDirectLighting(worldPos, normal, albedoTex.rgb);

    // Calculate ambient lighting from spherical G-Buffer
    vec3 ambientLighting = CalculateAmbientLighting(albedoTex.rgb);

    // Combine lighting
    vec3 finalColor = ambientLighting + directLighting;

    // Add albedo base color for unlit areas
    finalColor += albedoTex.rgb * 0.1;

    // Apply tone mapping
    finalColor = ToneMapReinhard(finalColor);

    // Output final color
    FragColor = vec4(finalColor, 1.0);

    // Debug visualizations (uncomment to use)
    // FragColor = vec4(normal * 0.5 + 0.5, 1.0); // Normal visualization
    // FragColor = vec4(sphericalResult.rgb, 1.0); // Spherical G-Buffer only
    // FragColor = vec4(directLighting, 1.0); // Direct lighting only
    // FragColor = vec4(ambientLighting, 1.0); // Ambient lighting only
}
