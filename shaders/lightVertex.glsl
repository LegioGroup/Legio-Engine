#version 330 core
layout (location = 0) in vec3 vpos;
//layout (location = 1) in vec2 vtex;
layout (location = 1) in vec3 vnormal;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(vpos, 1.0));

    //TODO: Costly operation, move it over to the CPU whenever possible and pass it as an uniform    
    Normal = mat3(transpose(inverse(model))) * vnormal;
    gl_Position = projection * view * vec4(FragPos, 1.0);

}