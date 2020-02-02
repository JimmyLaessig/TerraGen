#version 440 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 TexCoord0;

uniform mat4 worldMatrix;
uniform mat4 worldViewProjectionMatrix;

out vec3 PositionWS;

void main(void)
{
    PositionWS = (worldMatrix * vec4(Position,1.0f)).xyz;
    gl_Position = worldViewProjectionMatrix * vec4(Position, 1);
}
