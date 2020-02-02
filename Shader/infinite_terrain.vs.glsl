#version 440 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 TexCoord0;
layout (location = 2) in mat4 InstanceMatrix;


uniform mat4 invViewProjectionMatrix;
uniform mat4 viewProjectionMatrix;
uniform mat4 worldMatrix;
uniform vec3 cameraPosition;

//out vec2 texcoords_FS;
//out vec3 WorldPosition;
out vec3 position_CS;


bool rayPlaneIntersection(vec3 rayOrigin, vec3 rayDirection, vec3 planeOrigin, vec3 planeNormal, out vec3 position)
{
    float denom = dot(planeNormal, rayDirection);
    if(denom > 1e-6)
    {
        float t = dot(planeOrigin - rayOrigin, planeNormal) / denom;
        position =  rayOrigin + t * rayDirection;
        return true;
    }
    return false;
}

void main(void)
{
//    vec3 planeUp        = vec3(0, 0, 1);
//    vec3 planeOrigin    = vec3(0, 0, 0);


//    vec4 wp0        = invViewProjectionMatrix * vec4(positions_VS.xy, 1, 1);
//    vec3 viewDir    = normalize(wp0.xyz / wp0.w - cameraPosition);

//    vec3 worldPosition;

//    if(!rayPlaneIntersection(cameraPosition, viewDir, planeOrigin, planeUp, worldPosition))
//    {
//        worldPosition = vec3(0, 0, 0);
//    }
//    else
//    {

//    }
    vec4 pos = InstanceMatrix * vec4(Position, 1);
    position_CS     = pos.xyz/pos.w;
    // texcoords_FS    = texcoords_VS;
    // WorldPosition   = worldPosition;
    // gl_Position  = viewProjectionMatrix * vec4(worldPosition, 1);
    // gl_Position  = vec4(positions_VS.xy, 0.5, 1);

    gl_Position = viewProjectionMatrix * vec4(position_CS, 1);
}
