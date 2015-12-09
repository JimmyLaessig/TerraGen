#version 440 core


layout(vertices = 4) out;

in vec3 position_CS[];
in vec2 texCoords_CS[];


out vec3 position_ES[];
out vec2 texCoords_ES[];

void main(void)
{
    // Set the control points (vertices) of the output patch
    position_ES[gl_InvocationID] = position_CS[gl_InvocationID];
    texCoords_ES[gl_InvocationID] = texCoords_CS[gl_InvocationID];

    gl_TessLevelOuter[0] = 2;
    gl_TessLevelOuter[1] = 2;
    gl_TessLevelOuter[2] = 2;
    gl_TessLevelInner[0] = 1;

    // TODO TESSELATE HERE
}

