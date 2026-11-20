#version 330 core
// Кресла и зал: Blinn-Phong + shadow mapping (bias + PCF 3x3).
// Uniform-имена согласовать с Алиханом (shaders/core/shadow).
in vec3 vFragPos;
in vec3 vNormal;
in vec2 vUV;
in vec4 vFragPosLight;

uniform vec3 uColor;
uniform int  uUseTexture;
uniform sampler2D uTex;         // unit 0
uniform sampler2D uShadowMap;   // unit 1 (depth texture)
uniform int  uShadowsOn;        // клавиша T
uniform vec3 uLightPos;
uniform vec3 uLightColor;
uniform vec3 uViewPos;

out vec4 FragColor;

float shadowFactor(vec3 N, vec3 L) {
    vec3 p = vFragPosLight.xyz / vFragPosLight.w;
    p = p * 0.5 + 0.5;                                   // NDC -> [0,1]
    if (p.z > 1.0 || p.x < 0.0 || p.x > 1.0 || p.y < 0.0 || p.y > 1.0) return 0.0;
    float bias = max(0.005 * (1.0 - dot(N, L)), 0.0007); // slope-scaled bias
    vec2 texel = 1.0 / vec2(textureSize(uShadowMap, 0));
    float shadow = 0.0;
    for (int x = -1; x <= 1; ++x)
        for (int y = -1; y <= 1; ++y) {
            float closest = texture(uShadowMap, p.xy + vec2(x, y) * texel).r;
            shadow += (p.z - bias > closest) ? 1.0 : 0.0;
        }
    return shadow / 9.0;                                 // PCF 3x3
}

void main() {
    vec3 base = uColor;
    if (uUseTexture == 1) {
        // процедурный ковёр: тёмно-красный, шахматка 0.5 м + лёгкая полоса
        float check = mod(floor(vFragPos.x * 2.0) + floor(vFragPos.z * 2.0), 2.0);
        base *= mix(vec3(0.42, 0.10, 0.14), vec3(0.34, 0.08, 0.12), check);
    }

    vec3 N = normalize(vNormal);
    vec3 L = normalize(uLightPos - vFragPos);
    vec3 V = normalize(uViewPos - vFragPos);
    vec3 H = normalize(L + V);

    vec3 ambient = 0.25 * base;
    float diff = max(dot(N, L), 0.0);
    float spec = pow(max(dot(N, H), 0.0), 48.0) * 0.35;
    float shadow = (uShadowsOn == 1) ? shadowFactor(N, L) : 0.0;

    vec3 color = ambient + (1.0 - shadow) * (diff * base + vec3(spec)) * uLightColor;
    FragColor = vec4(color, 1.0);
}