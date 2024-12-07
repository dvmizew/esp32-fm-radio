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

void handleJoystickInput() {
    static float frequency = 87.5;

    if (digitalRead(JOYSTICK_SW) == LOW) {
        frequency += 0.1;
        if (frequency > 108.0) frequency = 87.5;
        tuneRadio(frequency);
        delay(300);
    }

    if (digitalRead(BUTTON_PLAY) == LOW) {
        playAudioViaBluetooth();
        delay(300);
    }
}