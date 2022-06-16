#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;

layout(location = 0) out vec3 fragColor;

layout(push_constant) uniform Push
{
    mat4 transform;
    vec3 color;
} push;

void main() {
    gl_Position = push.transform * vec4(inPosition, 1.0);
    fragColor = inColor;
}