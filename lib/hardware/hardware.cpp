#include <Wire.h>
#include <TEA5767.h>
#include "hardware.h"
#include "radioHandle.h"
#include "bluetooth.h"

TwoWire I2C_display = TwoWire(1); // we use I2C1 for the display
Adafruit_SSD1306 display(128, 64, &I2C_display); // OLED display object
TEA5767 radio;

void setupHardware() {
    // Wire is for TEA5767 radio
    // I2C_display is for OLED display
    Wire.begin(); // radio init
    Wire.setClock(100000); // radio clock

    I2C_display.begin(DISPLAY_SDA_PIN, DISPLAY_SCL_PIN);

    if (!display.begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS)) {
        Serial.println(F("Error initializing display!"));
        while (true);
    }

    radio.init();
}

unsigned long lastDebounceTime = 0;

void handleJoystickInput() {
    unsigned long currentTime = millis();

    if (currentTime - lastDebounceTime > debounceDelay) {
        lastDebounceTime = currentTime;
        
        if (digitalRead(JOYSTICK_SW) == LOW) {
            frequency += 0.1;
            if (frequency > 108.0) 
                frequency = 87.5;
            tuneRadio(frequency);
        }

        if (digitalRead(BUTTON_PLAY) == LOW) {
            initializeBluetoothSpeaker();
        }
    }
}