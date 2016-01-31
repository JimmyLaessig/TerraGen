#version 440 core


in vec2 heightmap_texcoords_FS;
in vec2 color_texcoords_FS;

in vec3 worldNormal_FS;
in float gradient_FS;

uniform sampler2D heightmapTexture;
uniform float maxHeight;

uniform bool wireframeEnabled = false;

uniform float texcoordScale = 1;
uniform sampler2D grasTexture;
uniform sampler2D rockTexture;

uniform vec3 lightDirection_World = vec3(-1, -1, 0);

layout(location = 0) out vec4 fragColor;

vec3 ambientComponent()
{
    return vec3(0.3f);
}


float diffuseComponent(vec3 L, vec3 N)
{
    return clamp(dot(L, N), 0.0, 1.0);
}


float specularComponent(vec3 lightDirection, vec3 viewDirection, vec3 normal, float shininess)
{
    if(dot(normal, lightDirection) < 0.0)
        return 0.0;

    return pow(max(0.0, dot(reflect(-lightDirection, normal), viewDirection)), shininess);
}

void main()
{

    if(wireframeEnabled)
    {
        fragColor = vec4(0,0,0,1);
        return;
    }
    vec4 diffuseColor = texture2D(rockTexture,color_texcoords_FS);
    float gradient = dot(worldNormal_FS, vec3(0.0, 1.0, 0.0));

    diffuseColor.xyz  *= diffuseComponent(-lightDirection_World, normalize(worldNormal_FS));
    diffuseColor.a = 1.0;

    fragColor = diffuseColor;
    //fragColor = vec4(color_texcoords_FS, 0, 1);
    //vec4 fragColor = diffuseColor;
    //fragColor = vec4(normalize(worldNormal_FS), 1);
    //fragColor = vec4(gradient_FS, gradient_FS, gradient_FS, 1);
}
