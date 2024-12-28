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

class DisplayHandle {
public:
    DisplayHandle();
    ~DisplayHandle() = default;

    void initDisplay(); // initialize display with default settings
    void clearAndUpdate(); // clear display and set cursor to (0, 0)
    void printCustomMessage(const char *message);

    void printResourceUsage();
    void printSystemInfo();
    void printSPIFFSInfo();
    void printDateTime();

    // print methods from other modules
    void printWiFiNetworks();
    void printWiFiConnectionStatus();
    void printBluetoothInfo();
    void printRadioInfo();
    void printWeatherInfo();
    void printRDSInfo();
    void printBluetoothConnectionStatus();
    void printAudioInfo();

    // tasks
    static void printResourceUsageTask(void *pvParameters);
    static void printDateTimeTask(void *pvParameters);
    static void printBluetoothInfoTask(void *pvParameters);
    static void printRadioInfoTask(void *pvParameters);

    // start tasks
    void startPrintResourceUsageTask();
    void startPrintDateTimeTask();
    void startPrintBluetoothInfoTask();
    void startPrintRadioInfoTask();

    // getter
    bool isDisplayInitialized() const;

private:
    TwoWire I2C_display; // I2C bus for display
    Adafruit_SSD1306 display; // display object
    bool displayInitialized;
};

#endif