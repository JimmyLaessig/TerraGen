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
    /// Static instance of a camera
    /// </summary>
    static Camera* Main;

    /// <summary>
    /// Creates new Camera
    /// </summary>
    Camera();

    /// <summary>
    /// Destroys the camera
    /// </summary>
    ~Camera();

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
    glm::mat4 getViewMatrix();

    /// <summary>
    /// Returns the matrix used to transform an object into screen space
    /// </summary>
    /// <returns></returns>
    glm::mat4 getProjectionMatrix();

    /// <summary>
    /// Gets the position of the camera
    /// </summary>
    glm::vec3 getPosition();

    /// <summary>
    /// Sets the position to the given value
    /// </summary>
    void setPosition(glm::vec3 position);

    /// <summary>
    /// Translates the cameras position
    /// </summary>
    /// <param name="value">The offset added to the position</param>
    void translate(glm::vec3 value);

    /// <summary>
    /// Rotate the camera
    /// </summary>
    /// <param name="axis">The axis of the rotation</param>
    /// <param name="angle">The angle of the rotation</param>
    void rotate(glm::vec3 axis, float angle);

    /// <summary>
    /// Rotates the camera to look at the target
    /// </summary>
    /// <param name="position"></param>
    void lookAt(glm::vec3 target);

    /// <summary>
    /// Returns a vector pointing in the forward direction of this object in world space.
    /// Per default forward is pointing in the +z-direction in model space.
    /// </summary>
    /// <returns></returns>
    glm::vec3 forward();

    /// <summary>
    /// Returns a vector pointing in the up direction of this object in world space
    /// </summary>
    /// <returns></returns>
    glm::vec3 up();

    /// <summary>
    /// Returns a vector pointing in the up direction of this object in world space
    /// </summary>
    /// <returns></returns>
    glm::vec3 right();

private:

    glm::mat4 projectionMatrix;
    glm::vec3 position;
    glm::vec3 rotation;


    /// <summary>
    /// Normalizes the angles of the rotation
    /// </summary>
    void normalizeAngles();

    /// <summary>
    /// Calculates a matrix storing the rotation of the camera
    /// </summary>
    /// <returns></returns>
    glm::mat4 orientation();
};

