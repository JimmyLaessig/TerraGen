#version 440 core

layout(triangles, equal_spacing, ccw) in;

uniform mat4 modelViewProjectionMatrix;

in vec3 position_ES[];
in vec2 texcoords_ES[];

out vec2 texcoords_FS;

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
    vec2 texcoords = interpolate2D(texcoords_ES[0], texcoords_ES[1], texcoords_ES[2]);

    // TODO: Displace the vertex along the normalize

    // transform to NDC
    gl_Position = modelViewProjectionMatrix * vec4(position, 1);
    texcoords_FS = texcoords;
}

