#include <Adafruit_SSD1306.h>
#include "displayHandle.h"
#include "radioHandle.h"
#include "hardware.h"

void initDisplay() {
    if (!display.begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS)) {
        Serial.println(F("Error initializing display!"));
        while (true);
    }
    
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
}

void updateDisplay() {
    static char lastStation[16] = "";
    static int lastVolume = -1;
    static float lastFrequency = 87.5;

    if (strcmp(currentStation, lastStation) != 0 || volume != lastVolume || frequency != lastFrequency) {
        display.clearDisplay();
        display.setCursor(0, 0);
        display.println(F("Radio ESP32"));
        display.setCursor(0, 16);
        display.println(F("Station: "));
        display.println(currentStation);
        display.setCursor(0, 32);
        display.print(F("Vol: "));
        display.println(volume);
        display.setCursor(0, 48);
        display.print(F("Freq: "));
        display.println(frequency, 1);
        display.display();

        strcpy(lastStation, currentStation);
        lastVolume = volume;
        lastFrequency = frequency;
    }
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