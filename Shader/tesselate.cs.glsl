#version 440 core


layout(vertices = 4) out;

in vec3 position_CS[];
in vec2 texCoords_CS[];
uniform mat4 modelMatrix;
uniform vec3 eyePosWorld;

out vec3 position_ES[];
out vec2 texCoords_ES[];

float getTessLevel(float distance0, float distance1)
{
    float avg = (distance0 + distance1) / 2.0;
    if(avg <= 2.0)
        return 10.0;
    else if(avg <= 5.)
        return 7.0;
    else
        return 3.0;
}

void main(void)
{
    // Set the control points (vertices) of the output patch
    position_ES[gl_InvocationID] = position_CS[gl_InvocationID];
    texCoords_ES[gl_InvocationID] = texCoords_CS[gl_InvocationID];

    float distance0 = distance(eyePosWorld , position_CS[0]);
    float distance1 = distance(eyePosWorld , position_CS[1]);
    float distance2 = distance(eyePosWorld , position_CS[2]);
    gl_TessLevelOuter[0] = getTessLevel(distance1, distance2);
    gl_TessLevelOuter[1] = getTessLevel(distance2, distance0);
    gl_TessLevelOuter[2] = getTessLevel(distance0, distance1);
    gl_TessLevelInner[0] = gl_TessLevelOuter[2];


    // TODO TESSELATE HERE
}

