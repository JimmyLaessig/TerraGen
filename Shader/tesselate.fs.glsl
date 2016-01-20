#version 440 core


in vec2 texcoords_FS;
in vec3 worldNormal_FS;

uniform sampler2D heightmapTexture;
uniform float maxHeight;

uniform bool wireframeEnabled = false;

uniform float texcoordScale = 1;
uniform sampler2D colorTexture;

layout(location = 0) out vec4 fragColor;


vec3 calculateNormal(vec2 texcoords)
{
    vec2 texelSize = 1.0 / textureSize(heightmapTexture, 0);

    float nc = texture2D(heightmapTexture, texcoords).x * maxHeight;
    float nr = texture2D(heightmapTexture, texcoords + texelSize * vec2(1, 0)).x * maxHeight;
    float nu = texture2D(heightmapTexture, texcoords + texelSize * vec2(0, 1)).x * maxHeight;

    vec3 normal;
    normal.x = nc - nr;
    normal.y = 1.0;
    normal.z = nc - nu;

    normal = normal * 0.5 + 0.5;
    return normalize(normal);
}

void main()
{

    if(wireframeEnabled)
    {
        fragColor = vec4(0,0,0,1);
        return;
    }

    // TODO Lighting
    vec3 normal = calculateNormal(texcoords_FS);
    fragColor = vec4(normal, 1);
}
