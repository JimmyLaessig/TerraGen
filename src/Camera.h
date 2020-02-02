#pragma once
#include "Transform.h"


class Camera
{
public:

    float zNear;

    float zFar;

    float fov;

    float ratio;

    /// <summary>
    /// Creates new Camera
    /// </summary>
    Camera();

    /// <summary>
    /// Creates a new projection matrix with the given values
    /// </summary>
    /// <param name="fov">Field of View</param>
    /// <param name="ratio">Aspect Ratio</param>
    /// <param name="np">Near Plane</param>
    /// <param name="fp">Far Plane</param>
    void setProjectionMatrix(float fov, float ratio, float np, float fp);

    /// <summary>
    /// Returns the matrix used to transform an object into view space
    /// </summary>
   glm::mat4 getViewMatrix() const;

    /// <summary>
    /// Returns the matrix used to transform an object into screen space
    /// </summary>
    /// <returns></returns>
   glm::mat4 getProjectionMatrix() const;


    /// <summary>
    /// Rotates the camera to look at the target
    /// </summary>
    /// <param name="position"></param>
    void lookAt(glm::vec3 position, glm::vec3 target, glm::vec3 up = glm::vec3(0, 0, 1));


    Transform transform;


    static Camera* Main;
};
