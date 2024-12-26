#ifndef DISPLAY_H
#define DISPLAY_H

#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <time.h>
#include <esp_system.h> // for system information
#include <SPIFFS.h> // for SPIFFS information
#include "hardware.h" // for display pins
#include "wifiHandle.h" // for printing wifi networks
#include "bluetooth.h" // for printing bluetooth connection status

class DisplayHandle {
public:
    DisplayHandle();
    
    void initDisplay(); // initialize display with default settings
    void printCustomMessage(const char *message);

    void printResourceUsage();
    void printSystemInfo();
    void printSPIFFSInfo();
    void printDateTime();

    // print methods from other modules
    void printWiFiNetworks();
    void printWiFiConnectionStatus();
    void printWeatherInfo();
    void printRDSInfo();
    void printBluetoothConnectionStatus();
    void printAudioInfo();

    // getter
    bool isDisplayInitialized() const;

private:
    TwoWire I2C_display; // I2C bus for display
    Adafruit_SSD1306 display; // display object
    bool displayInitialized;

    void clearAndUpdate(); // clear display and set cursor to (0, 0)
};

#endif