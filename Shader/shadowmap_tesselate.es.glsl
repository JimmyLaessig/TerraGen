#version 440 core

layout(triangles, equal_spacing, ccw) in;

in vec3 position_ES[];
in vec2 heightmap_texcoords_ES[];

uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;

uniform sampler2D heightmapTexture;
uniform float maxHeight = 1.0f;

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

void main(void)
{
    vec3 position = interpolate3D(position_ES[0].xyz, position_ES[1].xyz, position_ES[2].xyz);
    vec2 heightmap_texcoords = interpolate2D(heightmap_texcoords_ES[0], heightmap_texcoords_ES[1], heightmap_texcoords_ES[2]);

    // Displace Vertex in model space
    float displacement = texture2D(heightmapTexture , heightmap_texcoords).x;
    position.y = displacement * maxHeight;

    // transform to NDC
    gl_Position = modelViewProjectionMatrix * vec4(position, 1.0);
}

