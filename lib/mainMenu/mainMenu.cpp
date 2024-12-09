#include "mainMenu.h"
#include "displayHandle.h"
#include "hardware.h"

#include <Adafruit_SSD1306.h>

extern Adafruit_SSD1306 display;

// Menu items
#define MENU_ITEMS 4
const char *menuItems[MENU_ITEMS] = {
    "Radio Control",
    "Bluetooth Audio",
    "WiFi Settings",
    "System Info"
};

int currentMenuIndex = 0;

void initMainMenu() {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println(F("Main Menu:"));
    display.display();
}

void handleMainMenu() {
    static unsigned long lastDebounceTime = 0;

    if (millis() - lastDebounceTime > debounceDelay) {
        lastDebounceTime = millis();

        // Handle joystick movement
        if (digitalRead(JOYSTICK_Y) == LOW) {
            currentMenuIndex = (currentMenuIndex + 1) % MENU_ITEMS;
        } else if (digitalRead(JOYSTICK_X) == LOW) {
            currentMenuIndex = (currentMenuIndex - 1 + MENU_ITEMS) % MENU_ITEMS;
        }

        // Handle select button
        if (digitalRead(JOYSTICK_SW) == LOW) {
            switch (currentMenuIndex) {
                case 0:
                    // Go to Radio Control
                    Serial.println(F("Entering Radio Control"));
                    return;
                case 1:
                    // Go to Bluetooth Audio
                    Serial.println(F("Entering Bluetooth Audio"));
                    return;
                case 2:
                    // Go to WiFi Settings
                    Serial.println(F("Entering WiFi Settings"));
                    return;
                case 3:
                    // Show System Info
                    Serial.println(F("Displaying System Info"));
                    return;
            }
        }

        // Update display
        display.clearDisplay();
        display.setCursor(0, 0);
        display.println(F("Main Menu:"));
        for (int i = 0; i < MENU_ITEMS; i++) {
            if (i == currentMenuIndex) {
                display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); // Highlight
            } else {
                display.setTextColor(SSD1306_WHITE, SSD1306_BLACK); // Normal
            }
            display.setCursor(0, 16 + (i * 10));
            display.println(menuItems[i]);
        }
        display.display();
    }
}