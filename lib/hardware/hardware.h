#ifndef HARDWARE_H
#define HARDWARE_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>

#define debounceDelay 50

// pin definitions
#define BUTTON_PLAY 0 // inca nu e definit

// joystick pins
#define JOYSTICK_X 9 // SD2
#define JOYSTICK_Y 10 // SD3
#define JOYSTICK_SW 11 // CMD

// OLED display pins
#define DISPLAY_SDA_PIN 21
#define DISPLAY_SCL_PIN 22

// MAX98357A amplifier pins
#define AMP_SCK 13 // Clock
// #define AMP_MISO 12
// #define AMP_MOSI 11
#define AMP_CS 12 // Chip select
#define AMP_SDA 39 // SN

// TEA5767 radio pins
#define TEA5767_SDA 17
#define TEA5767_SCL 16

extern TwoWire I2C_display;
extern Adafruit_SSD1306 display;

void setupHardware();
void showAvailableWiFiNetworks();
void connectToWifi(const char *ssid, const char *password);
void handleJoystickInput();

#endif