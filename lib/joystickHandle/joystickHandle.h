#ifndef JOYSTICKHANDLE_H
#define JOYSTICKHANDLE_H

#include <Arduino.h>
#include "hardware.h"

class JoystickHandle {

public:
    JoystickHandle();
    ~JoystickHandle() = default;
    
    void setupJoystick();
    void readJoystick();
    void testJoystick();

    // getters
    void getJoystickX(int &x) const;
    void getJoystickY(int &y) const;
    void getJoystickSW(int &sw) const;
    void getJoystickValues(int &x, int &y, int &sw) const;

    private:
    int xValue;
    int yValue;
    int swValue;
};

#endif