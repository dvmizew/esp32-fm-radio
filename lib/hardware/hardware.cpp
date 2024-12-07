#include "hardware.h"
#include "radioHandle.h"
#include "bluetooth.h"

void setupHardware() {
    pinMode(BUTTON_PLAY, INPUT_PULLUP);
    pinMode(JOYSTICK_SW, INPUT_PULLUP);
    Wire.begin(DISPLAY_SDA_PIN, DISPLAY_SCL_PIN);
}

void showAvailableWiFiNetworks() {
    // TODO
}

void connectToWifi() {
    // TODO
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
            bluetoothSpeaker();
        }
    }
}