#version 330 core
layout (location = 0) in vec3 vpos;
//layout (location = 1) in vec2 vtex;
layout (location = 1) in vec3 vnormal;

uniform mat4 transform;

void main()
{
    gl_Position = transform * vec4(vpos, 1.0);
} 