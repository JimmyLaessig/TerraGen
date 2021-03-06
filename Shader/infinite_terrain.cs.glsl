#version 440 core
layout(vertices = 3) out;

in vec3 position_CS[];

uniform mat4 worldMatrix;
uniform vec3 cameraPosition;

uniform bool dynamicLodEnabled = false;

out vec3 position_ES[];
//out vec2 heightmap_texcoords_ES[];
//out vec2 color_texcoords_ES[];


float getTessLevel(float distance0, float distance1)
{
    float avg = (distance0 + distance1) / 2.0;
    if(avg <= 2.0)
        return 10.0;
    else if(avg <= 5.)
        return 7.0;
    else if(avg <= 25)
        return 3.0;
    return 1;
}


void main(void)
{
    // Set the control points (vertices) of the output patch
    position_ES[gl_InvocationID] = position_CS[gl_InvocationID];
    // heightmap_texcoords_ES[gl_InvocationID] = heightmap_texcoords_CS[gl_InvocationID];
    // color_texcoords_ES[gl_InvocationID]     = color_texcoords_CS[gl_InvocationID];

    float distance0 = distance(cameraPosition , (worldMatrix * vec4(position_CS[0], 1)).xyz);
    float distance1 = distance(cameraPosition , (worldMatrix * vec4(position_CS[1], 1)).xyz);
    float distance2 = distance(cameraPosition , (worldMatrix * vec4(position_CS[2], 1)).xyz);

    if(dynamicLodEnabled)
    {
       gl_TessLevelOuter[0] = getTessLevel(distance1, distance2);
       gl_TessLevelOuter[1] = getTessLevel(distance2, distance0);
       gl_TessLevelOuter[2] = getTessLevel(distance0, distance1);
       gl_TessLevelInner[0] = gl_TessLevelOuter[2];
    }
    else
    {
        gl_TessLevelOuter[0] = 1;
        gl_TessLevelOuter[1] = 1;
        gl_TessLevelOuter[2] = 1;
        gl_TessLevelInner[0] = 1;
    }
}

