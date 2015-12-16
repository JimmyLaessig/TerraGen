#include "keyevent.h"

KeyEvent::KeyEvent(){}

bool KeyEvent::isPressed()
{
    return keyPressed;
}

void KeyEvent::press()
{
    isPressed = true;
    keyPressedStartTime = QTime::currentTime();
}

void KeyEvent::release()
{
    isPressed = false;
}

double KeyEvent::keyDownDeltaTime()
{
    if(isPressed)
        return (QTime::currentTime() - keyPressedStartTime)/ 1000.0;

    return 0;
}
