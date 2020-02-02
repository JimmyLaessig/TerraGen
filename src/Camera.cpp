#include "Camera.h"
#include <cmath>
#define GLM_FORCE_RADIANS

Camera* Camera::Main = nullptr;

static const float MaxVerticalAngle = 85.0f; //must be less than 90 to avoid gimbal lock

Camera::Camera()
{
    Main = this;
}


void Camera::setProjectionMatrix(float fov, float ratio, float np, float fp)
{
    this->fov   = fov;
    this->ratio = ratio;
    this->zNear = np;
    this->zFar  = fp;
}


glm::mat4 Camera::getViewMatrix() const
{
    auto up		= this->transform.up();
    auto pos	= this->transform.getPosition();
    auto fw		= this->transform.forward();

    return glm::lookAt(pos, pos + fw, up);
}


glm::mat4 Camera::getProjectionMatrix() const
{
    return glm::perspective(glm::radians(fov), ratio, zNear, zFar);
}


void Camera::lookAt(glm::vec3 location, glm::vec3 target, glm::vec3 up)
{
    auto rot = glm::lookAt(location, target, up);
    this->transform.setRotation(glm::conjugate( glm::quat_cast(rot)));
    this->transform.setPosition(location);
}
