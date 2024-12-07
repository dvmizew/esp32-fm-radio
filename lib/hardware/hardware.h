#ifndef HARDWARE_H
#define HARDWARE_H

#include <Arduino.h>
#include <Wire.h>

#define debounceDelay 50

// pin definitions
#define BUTTON_PLAY 33

// joystick pins
#define JOYSTICK_X 34
#define JOYSTICK_Y 35
#define JOYSTICK_SW 32

// OLED display pins
#define DISPLAY_SDA_PIN 21
#define DISPLAY_SCL_PIN 22

// MAX98357A amplifier pins
#define AMP_SCK 14
#define AMP_MISO 12
#define AMP_MOSI 13
#define AMP_CS 15
#define AMP_SDA 26

void setupHardware();
void showAvailableWiFiNetworks();
void connectToWifi();
void handleJoystickInput();

#endif