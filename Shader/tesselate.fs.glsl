#version 440 core

in vec2 texcoords_FS;

uniform sampler2D colorTexture;

layout(location = 0) out vec4 fragColor;

void main()
{
        fragColor = vec4(0, 0, 0, 1);
        fragColor = vec4(texture2D(colorTexture, texcoords_FS).rgb,1);
}
