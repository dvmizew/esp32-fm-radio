#ifndef HARDWARE_H
#define HARDWARE_H

#include <Arduino.h>
#include <Wire.h>
#include <radioHandle.h>
#include <bluetooth.h>

// pin definitions
#define BUTTON_PLAY 33
#define JOYSTICK_X 34
#define JOYSTICK_Y 35
#define JOYSTICK_SW 32
#define SDA_PIN 21
#define SCL_PIN 22

void setupHardware();
void handleJoystickInput();

#endif