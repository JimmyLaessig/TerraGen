#ifndef KEYEVENT_H
#define KEYEVENT_H


class KeyEvent
{

public :
    KeyEvent();

    double keyDownDeltaTime();

    void press();
    void release();

    bool isPressed();

private:

    bool keyPressed;
    bool keyPressedStartTime;

}
#endif // KEYEVENT_H
