#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;

uniform mat4 modelViewProjectionMatrix = mat4(1);

out vec2 texUV;

void main() 
{
    texUV = texCoord;
    gl_Position =  modelViewProjectionMatrix * vec4(position, 1);
}
