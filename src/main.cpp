#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include "displayHandle.h"
#include "webserverHandle.h"
#include "hardware.h" // for DEVICE_NAME
#include "wifiHandle.h"

// unfortunately, my ESP32 can't handle the Bluetooth speaker and the web server at the same time
// because it has only 2 cores and 320KB of RAM which is not sufficient apparently (it is an Espressif ESP32-WROOM-32 38-pin module)
// so im doing what i can with what i have

SemaphoreHandle_t xMutex;
const char* AP_SSID = DEVICE_NAME;
const char* AP_PASSWORD = "1234";

void setup() {
    Serial.begin(115200);
    initDisplay();

    xMutex = xSemaphoreCreateMutex();
    if (xMutex == NULL) {
        Serial.println(F("Mutex creation failed"));
        return;
    }

    // start the device in AP mode
    startAP(AP_SSID, AP_PASSWORD); // ssid is the device name and password is 1234
    printAPInfo();
    String apInfo = "AP Mode\nSSID: " + String(DEVICE_NAME) + "\nPassword: 1234\nIP: " + WiFi.softAPIP().toString();
    displayCustomMessage(apInfo.c_str());
    Serial.printf_P(PSTR("Access point started\nClick here to open the web interface: http://%s\n"), WiFi.softAPIP().toString().c_str());

    // start the web server task
    startWebServerTask();
}

void loop() {
    vTaskDelay(pdMS_TO_TICKS(1000));
}