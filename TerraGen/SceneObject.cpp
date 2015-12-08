#include "SceneObject.h"


SceneObject::SceneObject()
{
    modelMatrix = glm::mat4(1);
}


SceneObject::~SceneObject()
{

}


void SceneObject::update(float deltaT)
{

}


void SceneObject::rotate(glm::vec3 axis, float angle)
{
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1), glm::radians(angle), axis);
    modelMatrix = modelMatrix * rotationMatrix;
}


void SceneObject::rotateWorld(glm::vec3 axis, float angle)
{
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1), glm::radians(angle), axis);
    modelMatrix = rotationMatrix * modelMatrix;
}


void SceneObject::translate(glm::vec3 translation)
{
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1), translation);
    modelMatrix = modelMatrix * translationMatrix;
}


void SceneObject::scale(glm::vec3 scale)
{
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1), scale);
    modelMatrix = modelMatrix * scaleMatrix;
}


void SceneObject::setPosition(glm::vec3 position)
{
    //modelMatrix -= glm::translate(glm::mat4(0),getPosition());
    modelMatrix[3][0] = 0;
    modelMatrix[3][1] = 0;
    modelMatrix[3][2] = 0;

    translate(position);
    //this->position = position;
}


void SceneObject::setRotation(glm::vec3 eulerAngles)
{
    //this->rotation = eulerAngles;
}


glm::vec3 SceneObject::getPosition()
{
    return glm::vec3(modelMatrix * glm::vec4(0, 0, 0, 1));
}


glm::vec3 SceneObject::getRotation()
{
    return glm::vec3(1);
}


glm::mat3 SceneObject::getNormalMatrix()
{
    return glm::mat3(glm::transpose(getInverseModelMatrix()));
}


glm::mat4 SceneObject::getInverseModelMatrix()
{
    return glm::inverse(modelMatrix);
}
