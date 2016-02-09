#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H
#include "glm/ext.hpp"
#include "glm/glm.hpp"

class DirectionalLight
{
public:
    DirectionalLight(glm::vec3 direction, glm::vec3 lightColor);
    ~DirectionalLight();

   glm::vec3 direction;
   glm::vec3 lightColor;

   glm::mat4 getViewMatrix();
   glm::mat4 getProjectionMatrix();
};

#endif // DIRECTIONALLIGHT_H
