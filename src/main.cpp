#include <Arduino.h>
#include "displayHandle.h"

void setup() {
    Serial.begin(115200);
    initDisplay();
    printCustomMessage("Hello, world!");
}

void loop() {
    
}