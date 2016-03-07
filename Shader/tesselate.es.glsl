#version 440 core

layout(triangles, equal_spacing, ccw) in;

in vec3 position_ES[];
in vec2 heightmap_texcoords_ES[];
in vec2 color_texcoords_ES[];

uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;

uniform mat4 depthMVPMatrix;

uniform sampler2D heightmapTexture;
uniform float maxHeight = 1.0f;
uniform vec2 numTiles;

out vec2 heightmap_texcoords_FS;
out vec2 color_texcoords_FS;
out vec3 worldNormal_FS;
out vec4 shadowMap_texcoords_FS;

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
    vec2 texelSize = (1.0 / textureSize(heightmapTexture, 0));

    float x_min = texture2D(heightmapTexture, texcoords + texelSize * vec2(-1, 0)).x * maxHeight;
    float x_pos = texture2D(heightmapTexture, texcoords + texelSize * vec2( 1, 0)).x * maxHeight;
    float z_min = texture2D(heightmapTexture, texcoords + texelSize * vec2( 0,-1)).x * maxHeight;
    float z_pos = texture2D(heightmapTexture, texcoords + texelSize * vec2( 0, 1)).x * maxHeight;

    vec3 normal;
    normal.x = x_min - x_pos;
    normal.y = 2.0;
    normal.z = z_min - z_pos;

    return normalize(normal);
}

void main(void)
{
    vec3 position = interpolate3D(position_ES[0].xyz, position_ES[1].xyz, position_ES[2].xyz);
    vec2 heightmap_texcoords = interpolate2D(heightmap_texcoords_ES[0], heightmap_texcoords_ES[1], heightmap_texcoords_ES[2]);
    vec2 color_texcoords = interpolate2D(color_texcoords_ES[0], color_texcoords_ES[1], color_texcoords_ES[2]);

    // Displace Vertex in model space
    float displacement = texture2D(heightmapTexture , heightmap_texcoords).x;
    position.y = displacement * maxHeight;

    // Normal in model space
    vec3 normal = calculateNormal(heightmap_texcoords);

    // transform to NDC
    gl_Position = modelViewProjectionMatrix * vec4(position, 1.0);

    heightmap_texcoords_FS = heightmap_texcoords;
    color_texcoords_FS = color_texcoords;
    worldNormal_FS =  normalize(normalMatrix * normal);

    shadowMap_texcoords_FS = depthMVPMatrix * vec4(position, 1.0);
}

