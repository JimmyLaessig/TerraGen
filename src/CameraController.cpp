#include "cameracontroller.h"
#include <QTime>


KeyEvent::KeyEvent(Qt::Key key)
{
    isPressed = false;
}


void KeyEvent::keyPressed()
{
    isPressed = true;
    keyPressedTime.start();
}


void KeyEvent::keyReleased()
{
    isPressed = false;
}


float KeyEvent::keyPressedDeltaTime()
{
    if(isPressed)
    {
        float deltaT = (float)keyPressedTime.elapsed() / 1000.0f;
        return deltaT;
    }
    return 0.0f;
}


CameraController::CameraController() :
    keyW(Qt::Key_W),
    keyA(Qt::Key_A),
    keyS(Qt::Key_S),
    keyD(Qt::Key_D)
{
    moveSpeed = 5.0f;
    rotationSpeed = 90.0f;
}

void CameraController::mouseMoved(glm::vec2 delta)
{
    eulerAngles.x = 0;
    eulerAngles.y += delta.y * rotationSpeed;
    eulerAngles.z += delta.x * rotationSpeed;
    eulerAngles.y = glm::clamp(eulerAngles.y, -89.9f, 89.9f);

    if (eulerAngles.z >= 360.0f)
    {
        eulerAngles.z -= 360.0f;
    }
    if (eulerAngles.z < 0.0f)
    {
        eulerAngles.z += 360.0f;
    }

    // Rotate around up axis
    auto rotZ = glm::angleAxis(glm::radians(eulerAngles.z), glm::vec3(0, 0, 1));
    // Rotate y-axis around the rotated z-axis
    auto yAxis = rotZ * glm::vec3(0, -1, 0);
    // Rotate around rotated yAxis
    auto rotY = glm::angleAxis(glm::radians(eulerAngles.y), yAxis);

    camera->transform.setRotation(rotY * rotZ);
}


void CameraController::setCamera(Camera* camera)
{
    this->camera        = camera;
    this->eulerAngles   = camera->transform.getEulerAngles();
}


void CameraController::keyReleased(int key)
{
    if (key == Qt::Key_W)keyW.keyReleased();
    if (key == Qt::Key_A)keyA.keyReleased();
    if (key == Qt::Key_S)keyS.keyReleased();
    if (key == Qt::Key_D)keyD.keyReleased();
}


void CameraController::keyPressed(int key)
{
    glm::vec3 delta(0);


    if (key == Qt::Key_W)
    {
        // Poll time between updates
        float deltaT = keyW.keyPressedDeltaTime();
        deltaT = 0.1f;
        // Trigger pressed Event
        keyW.keyPressed();

        delta += camera->transform.forward() * moveSpeed * deltaT;

    }
    if (key == Qt::Key_A)
    {
        // Poll time between updates
        float deltaT = keyA.keyPressedDeltaTime();
        deltaT = 0.1f;
        // Trigger pressed Event
        keyA.keyPressed();

        delta -= camera->transform.right() * moveSpeed * deltaT;
    }
    if (key == Qt::Key_S)
    {
        // Poll time between updates
        float deltaT = keyS.keyPressedDeltaTime();
        deltaT = 0.1f;
        // Trigger pressed Event
        keyS.keyPressed();

        delta -= camera->transform.forward() * moveSpeed * deltaT;
    }
    if (key == Qt::Key_D)
    {
        // Poll time between updates
        float deltaT = keyD.keyPressedDeltaTime();
        deltaT = 0.1f;
        // Trigger pressed Event
        keyD.keyPressed();

         delta += camera->transform.right() * moveSpeed * deltaT;
    }
    camera->transform.setPosition(camera->transform.getPosition() + delta);
}
