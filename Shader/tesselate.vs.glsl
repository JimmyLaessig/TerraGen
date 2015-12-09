#version 440 core

layout (location = 0) in vec3 position_VS;
layout (location = 1) in vec2 texCoords_VS;



out vec3 position_CS;
out vec2 texCoords_CS;

void main(void)
{
    position_CS = position_VS ;
    texCoords_CS =  texCoords_VS;
}
