#include <Arduino.h>
#include "lvgl.h"
#include "hardware.h"
#include "displayHandle.h"
#include "radioHandle.h"
#include "bluetooth.h"
#include "wifiHandle.h"
#include "webserver.h"
#include "mainMenu.h"

void setup() {
    Serial.begin(115200);
    setupHardware();
    
    // display stuff
    lv_init(); // initialize LittlevGL
    initDisplay(); // initialize OLED display

    initRadio(); // initialize radio module
    setupBluetooth();
    // web server stuff
    setupWebServer();
    initSPIFFS(); // init SPIFFS for web server
    
    initMainMenu();
}

void loop() {
    handleJoystickInput();
    handleMainMenu();
}