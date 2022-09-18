#version 330 core

out vec4 FragColor;

in vec3 fcolor;
in vec2 ftexcoord;

uniform sampler2D fTexture;
void main()
{
   FragColor = texture(fTexture, ftexcoord) * vec4(fcolor, 1.0);
}