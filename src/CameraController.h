#pragma once
#include "Camera.h"


#define _USE_MATH_DEFINES
#include <math.h> //for PI
#include <Qt>

#include <QCursor>
#include <QPoint>
#include <QTime>

#pragma once


class KeyEvent{

public:
    KeyEvent(Qt::Key key);

    void keyPressed();
    void keyReleased();

    float keyPressedDeltaTime();

private:
    bool isPressed;
    int key;
    QTime keyPressedTime;
};


/// <summary>
/// This class lets the user control the camera using the traditional WASD + mouse input
/// </summary>
class CameraController
{

public:

    /// <summary>
    /// Creates a new CameraController
    /// </summary>
    CameraController();


    void mouseMoved(glm::vec2 delta);


    void keyPressed(int key);


    void keyReleased(int key);


    /// <summary>
    /// Sets the camera object to the given camera
    /// </summary>
    /// <param name="camera">the camera to control</param>
    void setCamera(Camera* camera);


    /// <summary>
    /// Determines whether or not the controller is enabled
    /// </summary>
    bool enabled;

private:

    glm::vec3 eulerAngles = glm::vec3(0);

    KeyEvent keyW;
    KeyEvent keyA;
    KeyEvent keyS;
    KeyEvent keyD;


    /// <summary>
    /// Speed of movement
    /// </summary>
    float moveSpeed;


    /// <summary>
    /// speed of rotation
    /// </summary>
    float rotationSpeed;


    /// <summary>
    /// The camera that is controlled in the scene
    /// </summary>
    Camera* camera;

};

