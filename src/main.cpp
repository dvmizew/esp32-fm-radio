#include "displayHandle.h"
#include "webserverHandle.h"

void setup() {
    Serial.begin(115200);
    initDisplay();
    displayCustomMessage("X-PERT Production\nCioc Music\nCBX the family\nAlex Botea\n");
    setupWebServer();
    // initializeBluetoothSpeaker();
}

void loop() {
    // keep the loop function running to reset the watchdog timer
    delay(100);
}