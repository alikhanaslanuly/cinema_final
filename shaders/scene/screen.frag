#version 330 core
// Экран: анимированный процедурный «кадр», светится (emissive, без теней).
in vec2 vUV;
uniform float uTime;
out vec4 FragColor;

void main() {
    vec2 uv = vUV;
    vec3 c = 0.5 + 0.5 * cos(uTime * 0.6 + uv.xyx * 3.0 + vec3(0.0, 2.0, 4.0));
    float bars = smoothstep(0.46, 0.5, abs(fract(uv.x * 6.0 - uTime * 0.3) - 0.5));
    c = mix(c, vec3(1.0), 0.15 * bars);
    float vig = smoothstep(0.9, 0.3, length(uv - 0.5));
    FragColor = vec4(c * (0.6 + 0.6 * vig), 1.0);
}