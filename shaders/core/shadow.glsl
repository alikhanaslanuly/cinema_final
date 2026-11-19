// Shadow lookup (pass 2). Included by blinn_phong.frag via  #include "shadow.glsl".
//
//   p       = (uLightSpace * worldPos).xyz / w * 0.5 + 0.5      position in the shadow map, 0..1
//   cur     = linearDepth(p.z)                                  distance from the light, metres
//   stored  = linearDepth(texture(shadowMap, p.xy).r)           nearest occluder, metres
//   shadow  = cur - bias > stored
//   PCF     = average of that test over a (2r+1) x (2r+1) kernel of texels (r = uPcfRadius, 1 -> 3x3)
//   bias    = texelWorld * (uBiasTexels + (r + 0.5) * min(tan(theta), 5))   (slope-scaled, in metres)
//   texelWorld = uTexelScale * cur  =  size of one shadow texel at that distance
uniform sampler2D uShadowMap;
uniform mat4 uLightSpace;
uniform float uLightNear;
uniform float uLightFar;
uniform float uTexelScale;  // 2 * tan(fov / 2) / resolution
uniform float uBiasTexels;  // constant part of the bias, in texels (keys [ and ])
uniform int uPcfRadius;     // 0 = 1x1, 1 = 3x3 (default), 2 = 5x5 (key K)

// perspective depth buffer value (0..1) -> distance along the light axis in metres
float linearDepth(float d) {
    float z = d * 2.0 - 1.0;
    return (2.0 * uLightNear * uLightFar) / (uLightFar + uLightNear - z * (uLightFar - uLightNear));
}

// returns 0.0 = fully lit, 1.0 = fully in shadow
float shadowFactor(vec3 worldPos, vec3 N, vec3 L) {
    vec4 ls = uLightSpace * vec4(worldPos, 1.0);
    if (ls.w <= 0.0) return 0.0;
    vec3 p = ls.xyz / ls.w * 0.5 + 0.5;
    if (p.x < 0.0 || p.x > 1.0 || p.y < 0.0 || p.y > 1.0 || p.z > 1.0) return 0.0;  // outside the light frustum

    float cur = linearDepth(p.z);
    float cosT = clamp(dot(N, L), 0.0, 1.0);
    float tanT = sqrt(1.0 - cosT * cosT) / max(cosT, 0.05);
    float texelWorld = uTexelScale * cur;
    float bias = texelWorld * (uBiasTexels + (float(uPcfRadius) + 0.5) * min(tanT, 5.0));

    vec2 texel = 1.0 / vec2(textureSize(uShadowMap, 0));
    float sum = 0.0;
    for (int x = -uPcfRadius; x <= uPcfRadius; ++x) {
        for (int y = -uPcfRadius; y <= uPcfRadius; ++y) {
            float stored = linearDepth(texture(uShadowMap, p.xy + vec2(x, y) * texel).r);
            sum += (cur - bias > stored) ? 1.0 : 0.0;
        }
    }
    float n = float(2 * uPcfRadius + 1);
    return sum / (n * n);
}
