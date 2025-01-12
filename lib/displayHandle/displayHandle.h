#ifndef DISPLAY_H
#define DISPLAY_H

#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <time.h>
#include <esp_system.h> // for system information
#include <SPIFFS.h> // for SPIFFS information
#include <freertos/FreeRTOS.h>
#include <freertos/task.h> // for multithreading
#include "hardware.h" // for display pins
#include "wifiHandle.h" // for printing wifi networks
#include "radioHandle.h" // for printing radio information

#define I2C_FREQUENCY 400000
#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64
#define TEXT_SIZE_SMALL 1
#define TEXT_SIZE_LARGE 2

extern TwoWire I2C_display; // I2C bus 1
extern Adafruit_SSD1306 display; // 128x64 display

void initDisplay(); // initialize display with default settings
void clearAndUpdate(); // clear display and set cursor to (0, 0)
void displayCustomMessage(const char *message);

void displayResourceUsage();
void displaySystemInfo();
void displaySPIFFSInfo();
void displayCurrentDateTime();

// print methods from other modules
void displayWiFiNetworks();
void displayWiFiConnectionStatus();
void displayRadioInfo();
void displayWeatherInfo();
void displayRDSInfo();
void displayAudioInfo();

// tasks
void displayResourceUsageTask(void *pvParameters);
void displayDateTimeTask(void *pvParameters);
void displayRadioInfoTask(void *pvParameters);

// start tasks
void displayPrintResourceUsageTask();
void displayPrintDateTimeTask();
void startRadioInfoDisplayTask();

// getter
bool isDisplayInitialized();

#endif