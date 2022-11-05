#version 330 core

out vec4 FragColor;

in vec3 fcolor;
in vec2 ftexcoord;

uniform sampler2D fTexture1;
uniform sampler2D fTexture2;
uniform sampler2D fTexture3;
void main()
{
   //FragColor = texture(fTexture, ftexcoord) * vec4(fcolor, 1.0);
   FragColor =mix(mix(texture(fTexture1, ftexcoord), texture(fTexture2, ftexcoord), 0.5), texture(fTexture3, ftexcoord), 0.5);// * vec4(fcolor, 1.0);

}