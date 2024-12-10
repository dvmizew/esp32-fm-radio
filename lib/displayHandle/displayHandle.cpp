#include "displayHandle.h"
// #include "hardware.h"

DisplayHandle::DisplayHandle()
    : I2C_display(1), // I2C bus 1
      display(128, 64, &I2C_display), // 128x64 display
      displayInitialized(false) {} // we init the display in the main program

void DisplayHandle::initDisplay() {
    I2C_display.begin(DISPLAY_SDA_PIN, DISPLAY_SCL_PIN, 400000); // start I2C bus with SDA and SCL pins
    if (!display.begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS)) {
        Serial.println(F("Error initializing display!"));
        while(true);
    }

    display.clearDisplay(); // clear display buffer
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    displayInitialized = true;
}

void DisplayHandle::printCustomMessage(const char *message) {
    clearAndUpdate();
    display.setCursor(0, 0);
    display.println(message);
    display.display();
}

// print methods from other modules
void DisplayHandle::printWiFiNetworks() {
    // TODO
}

void DisplayHandle::printWiFiConnectionStatus() {
    // TODO
}

void DisplayHandle::printWeatherInfo() {
    // TODO
}

void DisplayHandle::printRDSInfo() {
    // TODO
}

void DisplayHandle::printBluetoothConnectionStatus() {
    // TODO
}

// status getter for display
bool DisplayHandle::isDisplayInitialized() const {
    return displayInitialized;
}

void DisplayHandle::clearAndUpdate() {
    display.clearDisplay();
    display.setCursor(0, 0);
}