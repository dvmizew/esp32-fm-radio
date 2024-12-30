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
    void getJoystickSW(uint8_t &sw) const;
    void getJoystickValues(int &x, int &y, uint8_t &sw) const;

    private:
    int xValue;
    int yValue;
    uint8_t swValue;
};

#endif