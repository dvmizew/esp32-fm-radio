#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include "displayHandle.h"
#include "hardware.h"

TwoWire I2C_display = TwoWire(1); // we use I2C1 for the display
Adafruit_SSD1306 display(128, 64, &I2C_display);

void initDisplay() {
    I2C_display.begin(DISPLAY_SDA_PIN, DISPLAY_SCL_PIN, 400000); // initialize I2C bus with pins and frequency
    if (!display.begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS)) {
        // if display initialization fails, print error message and stop execution
        Serial.println(F("Error initializing display!"));
        while (true);
    }
    
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
}

void printCustomMessage(const char *message) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println(message);
    display.display();
}

void printWiFiNetworks() {
    // TODO
}

void printWiFiConnectionStatus() {
    // TODO
}

void printWeatherInfo() {
    // TODO
}

void printRDSInfo() {
    // TODO
}

void printBluetoothConnectionStatus() {
    // TODO
}