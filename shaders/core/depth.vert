#version 330 core
// Shadow pass 1: the scene as seen from the light. Only the depth is stored.
layout(location = 0) in vec3 aPos;
uniform mat4 uModel;
uniform mat4 uLightSpace;  // light projection * light view
void main() {
    gl_Position = uLightSpace * uModel * vec4(aPos, 1.0);
}
