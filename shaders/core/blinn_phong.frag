#version 330 core
// Pass 2 fragment stage: Blinn-Phong spot light + hemispheric ambient + shadow map.
in vec3 vWorldPos;
in vec3 vNormal;
in vec2 vUV;
out vec4 FragColor;

// per-draw (set by Mesh / SeatGrid / Screen: uModel, uColor, uEmissive, uUseTexture)
uniform vec3 uColor;
uniform vec3 uEmissive;
uniform int uUseTexture;
uniform sampler2D uTexture;  // unit 0
uniform float uShininess;
uniform float uSpecStrength;

// per-frame (set by main)
uniform vec3 uViewPos;
uniform vec3 uLightPos;
uniform vec3 uLightDir;  // unit vector, from the lamp
uniform vec3 uLightColor;
uniform float uCosInner;
uniform float uCosOuter;
uniform vec3 uAmbientSky;
uniform vec3 uAmbientGround;
uniform int uShadowsOn;

#include "shadow.glsl"

void main() {
    vec3 N = normalize(vNormal);
    vec3 toLight = uLightPos - vWorldPos;
    float dist = length(toLight);
    vec3 L = toLight / dist;
    vec3 V = normalize(uViewPos - vWorldPos);
    vec3 H = normalize(L + V);  // Blinn half vector

    vec3 albedo = uColor;
    if (uUseTexture != 0) albedo *= texture(uTexture, vUV).rgb;

    float diff = max(dot(N, L), 0.0);
    float spec = diff > 0.0 ? pow(max(dot(N, H), 0.0), uShininess) * uSpecStrength : 0.0;

    float atten = 1.0 / (1.0 + 0.03 * dist + 0.004 * dist * dist);
    float spot = smoothstep(uCosOuter, uCosInner, dot(-L, normalize(uLightDir)));

    float shadow = (uShadowsOn != 0) ? shadowFactor(vWorldPos, N, L) : 0.0;

    vec3 ambient = mix(uAmbientGround, uAmbientSky, N.y * 0.5 + 0.5);
    vec3 direct = uLightColor * atten * spot * (1.0 - shadow);

    vec3 color = albedo * (ambient + direct * diff) + direct * spec + uEmissive;
    FragColor = vec4(pow(clamp(color, 0.0, 1.0), vec3(1.0 / 2.2)), 1.0);
}
