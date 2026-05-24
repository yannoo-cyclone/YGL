#version 330 core

// Spherical G-Buffer Propagation Vertex Shader
// Propagates radiosity through spherical coordinates using cube maps

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormal;

out vec3 vPosition;
out vec2 vTexCoord;
out vec3 vNormal;
out vec3 vWorldPosition;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform mat3 uNormalMatrix;

void main() {
    // Transform position to world space
    vec4 worldPos = uModel * vec4(aPosition, 1.0);
    vWorldPosition = worldPos.xyz;

    // Transform to view space
    vec4 viewPos = uView * worldPos;

    // Transform to clip space
    gl_Position = uProjection * viewPos;

    // Pass through attributes
    vPosition = aPosition;
    vTexCoord = aTexCoord;
    vNormal = normalize(uNormalMatrix * aNormal);
}
