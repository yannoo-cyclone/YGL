#version 330 core

// Spherical G-Buffer Propagation Fragment Shader
// Implements O(n) radiosity using spherical coordinates and cube maps

in vec3 vPosition;
in vec2 vTexCoord;
in vec3 vNormal;
in vec3 vWorldPosition;

out vec4 FragColor;

// Cube map textures for spherical G-Buffer
uniform samplerCube uEmitterBuffer;
uniform samplerCube uReceiverBuffer;

// Scene parameters
uniform vec3 uCameraPosition;
uniform float uSceneRadius;
uniform int uCurrentPass;
uniform int uMaxPasses;
uniform float uEnergyAttenuation;

// Material properties
uniform vec4 uAlbedo;
uniform float uRoughness;
uniform float uMetallic;
uniform vec3 uEmission;

// Convert world position to direction from camera
vec3 WorldToDirection(vec3 worldPos) {
    return normalize(worldPos - uCameraPosition);
}

// Sample from cube map using world direction
vec4 SampleCubeMap(samplerCube cubeMap, vec3 direction) {
    return texture(cubeMap, direction);
}

void main() {
    // Get direction from camera to this fragment
    vec3 viewDir = WorldToDirection(vWorldPosition);

    // Sample emitter buffer (previous pass radiosity)
    vec4 emitterRadiance = SampleCubeMap(uEmitterBuffer, viewDir);

    // Sample receiver buffer (geometry info)
    vec4 receiverInfo = SampleCubeMap(uReceiverBuffer, viewDir);

    // Calculate visibility and BRDF
    float NdotV = max(dot(vNormal, viewDir), 0.0);

    // Simple diffuse BRDF (Lambertian)
    float brdf = NdotV * (1.0 / 3.14159265359);

    // Attenuate based on distance from camera
    float distance = length(vWorldPosition - uCameraPosition);
    float attenuation = exp(-uEnergyAttenuation * distance * distance);

    // Combine emitter radiosity with surface properties
    vec3 outgoingRadiance = emitterRadiance.rgb * uAlbedo.rgb * brdf * attenuation;

    // Add emission from the surface itself
    outgoingRadiance += uEmission;

    // For the first pass, store geometry info in receiver buffer
    if (uCurrentPass == 0) {
        // Encode position and normal in receiver buffer
        // RGB: position (normalized), A: normal encoding
        FragColor = vec4(normalize(vWorldPosition), 1.0);
    } else {
        // Subsequent passes: store propagated radiosity
        FragColor = vec4(outgoingRadiance, 1.0);
    }
}
