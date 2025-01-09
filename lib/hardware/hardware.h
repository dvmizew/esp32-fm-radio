#ifndef HARDWARE_H
#define HARDWARE_H

#include <Wire.h>
#include <Arduino.h>
#include <driver/i2s.h>

#define DEVICE_NAME "ESP32inatorul"

// pin definitions
#define INTERNAL_LED 2

// button pins
#define PLAY_BUTTON 35 // also used for mute toggle for radio
#define NEXT_BUTTON 32 // also used for volume up
#define PREV_BUTTON 34 // also used for volume down

#define BUTTON_DEBOUNCE_DELAY 200

// OLED display pins (I2C)
#define DISPLAY_SDA_PIN 21
#define DISPLAY_SCL_PIN 22
#define SSD1306_I2C_ADDRESS 0x3C

// MAX98357A amplifier pins (I2S)
#define AMP_LRC 25 // LRCLK
#define AMP_BLCK 13 // Clock
#define AMP_DIN 12 // Data in
#define AMP_GAIN 14
#define AMP_SD 33

extern i2s_config_t amp_i2s_config;
extern i2s_pin_config_t amp_config;

// TEA5767 radio pins (I2C)
#define TEA5767_SDA 17
#define TEA5767_SCL 16

// function prototypes
void setupButtons();
void testButtons();
void restartESP();
void blinkInternalLED();
void printI2CDevices();
void printSystemInfo();

#endif