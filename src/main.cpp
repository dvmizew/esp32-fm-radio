#include <Arduino.h>
#include "hardware.h"
#include "displayHandle.h"
#include "radioHandle.h"
#include "bluetooth.h"
#include "webserver.h"

void setup() {
    Serial.begin(115200);
    setupHardware();
    initDisplay();
    initRadio();
    setupBluetooth();
    setupWebServer();
    updateDisplay();
}

void loop() {
    handleJoystickInput();
    handleRadioControl();
    playThroughBluetoothSpeaker();
}