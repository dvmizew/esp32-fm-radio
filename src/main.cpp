#include "displayHandle.h"
#include "webserverHandle.h"
#include "hardware.h"
// #include "bluetooth.h"

DisplayHandle displayHandle;

void setup() {
    Serial.begin(115200);
    displayHandle.initDisplay();
    displayHandle.displayCustomMessage("X-PERT Production\nCioc Music\nCBX the family\nAlex Botea\n");
    setupWebServer();
    // initializeBluetoothSpeaker();
    displayHandle.displayResourceUsage();
}

void loop() {
    // keep the loop function running to reset the watchdog timer
    delay(100);
}