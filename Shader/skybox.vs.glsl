#version 440 core

layout (location = 0) in vec3 position;

uniform mat4 modelViewProjectionMatrix;

out vec3 texcoords;

void main(void)
{
    vec4 worldPos = modelViewProjectionMatrix * vec4(position, 1);
    gl_Position = worldPos;
    texcoords = position;
}
