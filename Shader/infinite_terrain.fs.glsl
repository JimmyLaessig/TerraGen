#version 440 core

in vec3 WorldPosition;
in vec3 WorldNormal;


uniform mat4 invProjectionMatrix;
uniform mat4 invViewMatrix;

uniform mat4 invViewProjectionMatrix;
uniform mat4 viewProjectionMatrix;
uniform mat4 worldMatrix;
uniform vec3 cameraPosition;
uniform int bWireframeEnabled;
//uniform bool wireframeEnabled = false;
//uniform sampler2D colorTexture;

layout(location = 0) out vec4 fragColor;


//bool rayPlaneIntersection(vec3 rayOrigin, vec3 rayDirection, vec3 planeOrigin, vec3 planeNormal, out vec3 position)
//{
//    float denom = dot(planeNormal, rayDirection);
//    if(denom > 1e-6)
//    {
//        float t = dot(planeOrigin - rayOrigin, planeNormal) / denom;
//        position =  rayOrigin + t * rayDirection;
//        return true;
//    }
//    return false;
//}


void main()
{

//    vec3 planeUp        = vec3(0, 0, 1);
//    vec3 planeOrigin    = vec3(0, 0, 0);
//    vec3 ssp        = vec3(gl_FragCoord.x , 1 - gl_FragCoord.y , 1);
//    vec4 vp         = invProjectionMatrix   * vec4(ssp, 1);
//    vec4 wp0        = invViewMatrix         * vec4(vp.xyz / vp.w,  1);
//    vec3 viewDir    = normalize(wp0.xyz / wp0.w - cameraPosition);

//    vec3 worldPosition;

//    if(!rayPlaneIntersection(cameraPosition, viewDir, planeOrigin, planeUp, worldPosition))
//    {
//         gl_FragDepth = 0.99f;
//         gl_FragColor = vec4(0, 0, 0, 1);
//    }
//    else
//    {
//        if(wireframeEnabled)
//        {
//            gl_FragColor = vec4(0,0,0,1);
//        }
//        else
//        {
//            vec4 position_ScreenSpace = viewProjectionMatrix * vec4(worldPosition, 1);
//            gl_FragDepth =  0.99;//abs(position_ScreenSpace.z / position_ScreenSpace.w);

//            gl_FragColor = vec4(fract(worldPosition.x) , fract(worldPosition.y), worldPosition.z, 1);// ;vec4(1, 0, 0, 1);
//            gl_FragColor = vec4(0 , 0, worldPosition.z, 1);// ;vec4(1, 0, 0, 1);
//            gl_FragColor = vec4(1 , 0, 0, 1);
//        }
//    }
    //float x = gl_InstanceID / 3;

    if(bWireframeEnabled == 1)
    {
        fragColor = vec4(0, 0, 0, 1);
    }
    else
    {
        fragColor = vec4(1, 0, 0, 1);
    }

}
