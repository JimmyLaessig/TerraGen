#version 440 core

in vec3 position;
in vec2 texcoords_FS;
in vec3 PositionWS;

uniform vec3 color = vec3(1, 1, 0);
uniform int bWireframeEnabled = 1;
out vec4 fragColor;

void main()
{
    if(bWireframeEnabled == 1)
    {
        fragColor = vec4(PositionWS, 1);
    }
    else
    {
        fragColor = vec4(0, 0, 0, 1);
    }
}
