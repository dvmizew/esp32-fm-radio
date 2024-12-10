#ifndef HARDWARE_H
#define HARDWARE_H

// pin definitions
#define BUTTON_PLAY 0 // inca nu e definit

// joystick pins
#define JOYSTICK_X 9 // SD2
#define JOYSTICK_Y 10 // SD3
#define JOYSTICK_SW 11 // CMD

// OLED display pins
#define DISPLAY_SDA_PIN 21
#define DISPLAY_SCL_PIN 22
#define SSD1306_I2C_ADDRESS 0x3C

// MAX98357A amplifier pins
#define AMP_SCK 13 // Clock
// #define AMP_MISO 12
// #define AMP_MOSI 11
#define AMP_CS 12 // Chip select
#define AMP_SDA 25

// TEA5767 radio pins
#define TEA5767_SDA 17
#define TEA5767_SCL 16

#endif