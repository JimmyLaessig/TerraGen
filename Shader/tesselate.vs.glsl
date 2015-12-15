#version 440 core

layout (location = 0) in vec3 position_VS;
layout (location = 1) in vec2 texcoords_VS;

uniform mat4 modelMatrix;

out vec3 position_CS;
out vec2 texcoords_CS;

void main(void)
{
    position_CS = vec3(modelMatrix * vec4(position_VS, 1));
    texcoords_CS =  texcoords_VS;
}
