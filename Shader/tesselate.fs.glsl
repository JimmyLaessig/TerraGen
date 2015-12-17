#version 440 core

in vec2 texcoords_FS;

uniform vec2 gridDimensions = vec2(1);
uniform vec2 gridCoords = vec2(1);

uniform bool wireframeEnabled = false;
uniform float texcoordScale = 1;
uniform sampler2D colorTexture;

layout(location = 0) out vec4 fragColor;

vec2 calculateTexcoords(vec2 texcoords, vec2 gridDimensions, vec2 gridCoords)
{
  //  texcoords.y = 1- texcoords.y;
    vec2 min = gridCoords / gridDimensions;
    vec2 max = (gridCoords +1)/ gridDimensions;
    return (max - min) * texcoords + min;
}

void main()
{

    if(wireframeEnabled)
    {
        fragColor = vec4(0,0,0,1);
        return;
    }

    // TODO Lighting
    vec2 texcoords =calculateTexcoords(texcoords_FS, gridDimensions, gridCoords) * texcoordScale;
    fragColor = vec4(texture2D(colorTexture, texcoords).rgb,1);
}
