#version 330 core
layout (location = 0) in vec3 vpos;
layout (location = 1) in vec3 vcolor;
layout (location = 2) in vec2 vtex;

out vec3 fcolor;
out vec2 ftexcoord;
void main()
{
   gl_Position = vec4(vpos, 1.0);
   fcolor = vcolor;
   ftexcoord = vtex;
}