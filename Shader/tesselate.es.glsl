#version 440 core

layout(triangles, equal_spacing, ccw) in;

uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;

uniform sampler2D heightmapTexture;
uniform float maxHeight = 1.0f;
uniform vec2 numTiles;
in vec3 position_ES[];
in vec2 texcoords_ES[];

out vec2 texcoords_FS;
out vec3 worldNormal_FS;
// Interpolate values v0-v2 based on the barycentric coordinates
// of the current vertex within the triangle
vec2 interpolate2D(vec2 v0, vec2 v1, vec2 v2)
{
    return  vec2(gl_TessCoord.x) * v0 +
            vec2(gl_TessCoord.y) * v1 +
            vec2(gl_TessCoord.z) * v2;
}

// Interpolate values v0-v2 based on the barycentric coordinates
// of the current vertex within the triangle
vec3 interpolate3D(vec3 v0, vec3 v1, vec3 v2)
{
    return  vec3(gl_TessCoord.x) * v0 +
            vec3(gl_TessCoord.y) * v1 +
            vec3(gl_TessCoord.z) * v2;
}


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


void main(void)
{
    vec3 position = interpolate3D(position_ES[0].xyz, position_ES[1].xyz, position_ES[2].xyz);
    vec2 texcoords = interpolate2D(texcoords_ES[0], texcoords_ES[1], texcoords_ES[2]);

    // Displace Vertex in model space
    float displacement = texture2D(heightmapTexture , texcoords).x;
    position.y = displacement * maxHeight;

    // Normal in model space
    vec3 normal = texture2D(heightmapTexture, texcoords).xyz;
    normal = calculateNormal(texcoords);
    normal = normalize(normal);

    // transform to NDC
    gl_Position = modelViewProjectionMatrix * vec4(position, 1);
    texcoords_FS = texcoords;
    worldNormal_FS =  normalize(normalMatrix * normal);
}

