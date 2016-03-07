#include "directionallight.h"

DirectionalLight::DirectionalLight(glm::vec3 position, glm::vec3 direction, glm::vec3 lightColor) :
    position(position),
    direction(direction),
    lightColor(lightColor)
{}

DirectionalLight::~DirectionalLight()
{}

glm::mat4 DirectionalLight::getViewMatrix()
{
    return glm::lookAt(position, position + direction, glm::vec3(0,1,0));
}

glm::mat4 DirectionalLight::getProjectionMatrix()
{
    return glm::ortho<float>(-20,20,-20,20,-20,40);
    //return glm::ortho<float>(-200.f, 200.f, -100.f, 200.f,-250.f,150.f);
}
