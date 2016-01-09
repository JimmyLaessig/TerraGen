#version 440 core

layout(triangles, equal_spacing, ccw) in;

uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;

uniform sampler2D heightmapTexture;
uniform float heightScale = 1.0f;

uniform vec2 gridDimensions = vec2(1);
uniform vec2 gridCoords = vec2(1);


in vec3 position_ES[];
in vec2 texcoords_ES[];

out vec2 texcoords_FS;
out vec3 viewNormal_FS;

// Scales the given texcoords (in domain [0,1]) to the dimensions of the grid at the given coordinates
vec2 scaleCoordinates(vec2 coordinates, vec2 gridDimensions, vec2 gridCoords)
{
  //  texcoords.y = 1- texcoords.y;
    vec2 min = gridCoords / gridDimensions;
    vec2 max = (gridCoords +1)/ gridDimensions;
    return (max - min) * coordinates + min;
}

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

vec3 calculateNormal(vec2 texcoords, float texelSize, float texelAspect)
{
    vec4 h;
    h.x = texture2D(heightmapTexture, texcoords + texelSize * vec2( 0,-1)).r * texelAspect;
    h.y = texture2D(heightmapTexture, texcoords + texelSize * vec2(-1, 0)).r * texelAspect;
    h.z = texture2D(heightmapTexture, texcoords + texelSize * vec2( 1, 0)).r * texelAspect;
    h.w = texture2D(heightmapTexture, texcoords + texelSize * vec2( 0, 1)).r * texelAspect;

    vec3 normal;
    normal.x = h.y - h.z;
    normal.z = h.x - h.w;
    normal.y = 2;

    return normalize(normal);

}

void main(void)
{
    vec3 position = interpolate3D(position_ES[0].xyz, position_ES[1].xyz, position_ES[2].xyz);
    vec2 texcoords = interpolate2D(texcoords_ES[0], texcoords_ES[1], texcoords_ES[2]);


    // Displace Vertex in model world space
    vec2 heightmapCoords = scaleCoordinates(texcoords, gridDimensions, gridCoords);
    float displacement = 1.0 - texture2D(heightmapTexture , heightmapCoords).x;
    position.y = displacement * heightScale;

    // transform to NDC
    gl_Position = modelViewProjectionMatrix * vec4(position, 1);
    texcoords_FS = texcoords;

}

