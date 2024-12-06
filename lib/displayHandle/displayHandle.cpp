#include "displayHandle.h"
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display(128, 64, &Wire);

void initDisplay() {
    if (!display.begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS)) {
        Serial.println("Error initializing display!");
        while (true);
    }
    
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("Radio ESP32");
    display.display();
}

void updateDisplay() {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Radio ESP32");
    display.setCursor(0, 16);
    display.println("Post: " + currentStation);
    display.setCursor(0, 32);
    display.print("Volum: ");
    display.println(volume);
    display.display();
}