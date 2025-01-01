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
#include "bluetooth.h" // for printing bluetooth connection status
#include "radioHandle.h" // for printing radio information

#define I2C_FREQUENCY 400000
#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64
#define TEXT_SIZE_SMALL 1
#define TEXT_SIZE_LARGE 2

class DisplayHandle {
public:
    DisplayHandle();
    ~DisplayHandle() = default;

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
    void displayBluetoothInfo();
    void displayRadioInfo();
    void displayWeatherInfo();
    void displayRDSInfo();
    void displayBluetoothConnectionStatus();
    void displayAudioInfo();

    // tasks
    static void displayResourceUsageTask(void *pvParameters);
    static void displayDateTimeTask(void *pvParameters);
    static void displayBluetoothInfoTask(void *pvParameters);
    static void displayRadioInfoTask(void *pvParameters);

    // start tasks
    void displayPrintResourceUsageTask();
    void displayPrintDateTimeTask();
    void displayPrintBluetoothInfoTask();
    void displayPrintRadioInfoTask();

    // getter
    bool isDisplayInitialized() const;

private:
    TwoWire I2C_display; // I2C bus for display
    Adafruit_SSD1306 display; // display object
    bool displayInitialized;
};

#endif