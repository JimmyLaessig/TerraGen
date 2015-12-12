#version 440 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec2 vTexcoords;


uniform mat4 modelViewProjectionMatrix;

out vec2 texcoords;

void main(void)
{
    gl_Position = modelViewProjectionMatrix * vec4(vPosition, 1);
    texcoords =  vTexcoords;
}
