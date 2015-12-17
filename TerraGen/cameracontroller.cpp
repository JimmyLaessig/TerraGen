#include "CameraController.h"
#include <QTime>
#define GLM_FORCE_RADIANS

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

CameraController::CameraController() : keyW(Qt::Key_W),keyA(Qt::Key_A), keyS(Qt::Key_S), keyD(Qt::Key_D)
{
    moveSpeed = 5.0f;
    rotationSpeed = 90.0f;
}

CameraController::~CameraController(){}

void CameraController::setCamera(Camera* camera)
{
    this->camera = camera;
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
    if (key == Qt::Key_W)
    {
        // Poll time between updates
        float deltaT = keyW.keyPressedDeltaTime();
        deltaT = 0.1f;
        // Trigger pressed Event
        keyW.keyPressed();

        camera->translate(camera->forward() * moveSpeed * deltaT);

    }
    if (key == Qt::Key_A)
    {
        // Poll time between updates
        float deltaT = keyA.keyPressedDeltaTime();
        deltaT = 0.1f;
        // Trigger pressed Event
        keyA.keyPressed();

        camera->translate(-camera->right() * moveSpeed * deltaT);
    }
    if (key == Qt::Key_S)
    {
        // Poll time between updates
        float deltaT = keyS.keyPressedDeltaTime();
        deltaT = 0.1f;
        // Trigger pressed Event
        keyS.keyPressed();

        camera->translate(-camera->forward() * moveSpeed * deltaT);
    }
    if (key == Qt::Key_D)
    {
        // Poll time between updates
        float deltaT = keyD.keyPressedDeltaTime();
        deltaT = 0.1f;
        // Trigger pressed Event
        keyD.keyPressed();

        camera->translate(camera->right() * moveSpeed * deltaT);
    }
}
