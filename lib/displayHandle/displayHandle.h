#ifndef DISPLAY_H
#define DISPLAY_H

#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include "hardware.h"

class DisplayHandle {
public:
    DisplayHandle();
    
    void initDisplay(); // initialize display with default settings
    void printCustomMessage(const char *message);

    // print methods from other modules
    void printWiFiNetworks();
    void printWiFiConnectionStatus();
    void printWeatherInfo();
    void printRDSInfo();
    void printBluetoothConnectionStatus();

    // getter
    bool isDisplayInitialized() const;

private:
    TwoWire I2C_display; // I2C bus for display
    Adafruit_SSD1306 display; // display object
    bool displayInitialized;

    void clearAndUpdate(); // clear display and set cursor to (0, 0)
};

#endif