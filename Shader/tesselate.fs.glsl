#version 440 core


in vec2 heightmap_texcoords_FS;
in vec2 color_texcoords_FS;
in vec4 shadowMap_texcoords_FS;
in vec3 worldNormal_FS;

uniform sampler2D heightmapTexture;
uniform float maxHeight;

uniform bool wireframeEnabled = false;

uniform sampler2D grasTexture;
uniform sampler2D rockTexture;

uniform sampler2D shadowMap;

uniform vec3 lightDirection_World = vec3(-1, -1, 0);

layout(location = 0) out vec4 fragColor;

bool inShadow()
{
vec2 uv = shadowMap_texcoords_FS.xy / shadowMap_texcoords_FS.w;
float depth_p = shadowMap_texcoords_FS.z / shadowMap_texcoords_FS.w;
float depth_c = texture2D(shadowMap, uv).r;

return (depth_c < depth_p);
}

vec3 ambientComponent()
{
    return vec3(0.3f);
}

float diffuseComponent(vec3 L, vec3 N)
{
    return clamp(dot(L, N), 0.0, 1.0);
}
//
float specularComponent(vec3 lightDirection, vec3 viewDirection, vec3 normal, float shininess)
{
    if(dot(normal, lightDirection) < 0.0)
        return 0.0;
    return pow(max(0.0, dot(reflect(-lightDirection, normal), viewDirection)), shininess);
}

// Determines the color of the fragment based on the gradient
vec4 calculateColor(float slope)
{
    float gradient_rock = 0.03;
    float gradient_mixed = 0.02;

    // Rock
    if(slope > gradient_rock)
    {
        return texture2D(rockTexture,color_texcoords_FS);
    }
    // Blend rock and grass
    if(slope > gradient_mixed)
    {
        vec4 rock = texture2D(rockTexture,color_texcoords_FS);
        vec4 grass = texture2D(grasTexture,color_texcoords_FS);

        float i = (slope - gradient_mixed) / (gradient_rock - gradient_mixed) ;

        return mix(grass, rock, i);
    }
    // Grass
    return texture2D(grasTexture,color_texcoords_FS);
}


void main()
{
    if(wireframeEnabled)
    {
        fragColor = vec4(0,0,0,1);
        return;
    }
    float slope = 1 - worldNormal_FS.y;
    vec4 color = calculateColor(slope);

    float diffuseFactor = diffuseComponent(-lightDirection_World, normalize(worldNormal_FS));

    //diffuseFactor *= inShadow() ? 0.0 : 1.0;

    fragColor.xyz = color.xyz * diffuseFactor;
    fragColor.a = color.a;
}
