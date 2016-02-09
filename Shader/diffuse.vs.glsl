#version 440 core

layout (location = 0) in vec3 positions_VS;
layout (location = 1) in vec2 texcoords_VS;


uniform mat4 modelViewProjectionMatrix;

out vec2 texcoords_FS;


void main(void)
{
   gl_Position = modelViewProjectionMatrix * vec4(positions_VS, 1);
    texcoords_FS =  texcoords_VS;
}
