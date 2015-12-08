#include "camera.h"
#include <cmath>
#define GLM_FORCE_RADIANS


static const float MaxVerticalAngle = 85.0f; //must be less than 90 to avoid gimbal lock

Camera::Camera()
{
    rotation = glm::vec3(0);
    position = glm::vec3(0);
}


Camera::~Camera()
{
}


void Camera::setProjectionMatrix(float fov, float ratio, float np, float fp)
{
    this->fov = fov;
    this->ratio = ratio;
    this->zNear = np;
    this->zFar = fp;
    projectionMatrix = glm::perspective(glm::radians(fov), ratio, np, fp);
}


glm::mat4 Camera::getViewMatrix()
{
    return orientation() * glm::translate(glm::mat4(1.0f), -position);
}


glm::mat4 Camera::getProjectionMatrix()
{
    return projectionMatrix;
}


glm::vec3 Camera::getPosition()
{
    return position;
}


void Camera::setPosition(glm::vec3 position)
{
    this->position = position;
}


void Camera::translate(glm::vec3 value)
{
    position += value;
}


void Camera::rotate(glm::vec3 axis, float angle)
{
    rotation += axis * angle;
    normalizeAngles();
}


glm::vec3 Camera::forward()
{
    glm::vec4 fw = glm::inverse(orientation()) * glm::vec4(0, 0, -1, 1);

    return glm::normalize(glm::vec3(fw));
}


glm::vec3 Camera::up()
{
    glm::vec4 fw = glm::inverse(orientation()) * glm::vec4(0, 1, 0, 1);

    return glm::normalize(glm::vec3(fw));
}


glm::vec3 Camera::right()
{
    glm::vec4 fw = glm::inverse(orientation()) * glm::vec4(1, 0, 0, 1);

    return glm::normalize(glm::vec3(fw));
}


void Camera::lookAt(glm::vec3 position)
{
    glm::vec3 direction = glm::normalize(this->position - position);
    rotation.x = glm::degrees(asinf(-direction.y));
    rotation.y = -glm::degrees(atan2f(-direction.x, -direction.z));
    normalizeAngles();
}


glm::mat4 Camera::orientation()
{
    glm::mat4 orientation;
    orientation = glm::rotate(orientation, glm::radians(rotation.x), glm::vec3(1, 0, 0));
    orientation = glm::rotate(orientation, glm::radians(rotation.y), glm::vec3(0, 1, 0));
    orientation = glm::rotate(orientation, glm::radians(rotation.z), glm::vec3(0, 0, 1));

    return orientation;
}


void Camera::normalizeAngles()
{
    rotation.x = fmodf(rotation.x, 360.0f);
    //fmodf can return negative values, but this will make them all positive
    if (rotation.y < 0.0f)
        rotation.y += 360.0f;
    if (rotation.y > 360.0f)
        rotation.y -= 360.0f;

    if (rotation.x > MaxVerticalAngle)
        rotation.x = MaxVerticalAngle;
    else if (rotation.x < -MaxVerticalAngle)
        rotation.x = -MaxVerticalAngle;

}

