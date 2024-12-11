#include "joystickHandle.h"

JoystickHandle::JoystickHandle() 
    : xValue(0), 
      yValue(0), 
      swValue(0) {}

void JoystickHandle::setupJoystick() {
    pinMode(JOYSTICK_X, INPUT);
    pinMode(JOYSTICK_Y, INPUT);
    pinMode(JOYSTICK_SW, INPUT);
}

void JoystickHandle::readJoystick() {
    xValue = analogRead(JOYSTICK_X);
    yValue = analogRead(JOYSTICK_Y);
    swValue = digitalRead(JOYSTICK_SW);
}

void JoystickHandle::testJoystick() {
    while (true) {
        readJoystick();
        Serial.print("X: ");
        Serial.print(xValue);
        Serial.print(" | Y: ");
        Serial.print(yValue);
        Serial.print(" | SW: ");
        Serial.println(swValue);
        delay(1000);
    }
}

// getters
void JoystickHandle::getJoystickX(int &x) const {
    x = xValue;
}

void JoystickHandle::getJoystickY(int &y) const {
    y = yValue;
}

void JoystickHandle::getJoystickSW(int &sw) const {
    sw = swValue;
}

void JoystickHandle::getJoystickValues(int &x, int &y, int &sw) const {
    x = xValue;
    y = yValue;
    sw = swValue;
}