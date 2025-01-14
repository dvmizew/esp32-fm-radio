#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include "displayHandle.h"
#include "webserverHandle.h"
#include "hardware.h"
#include "wifiHandle.h"
#include "bluetooth.h"

// unfortunately, my ESP32 can't handle the Bluetooth speaker and the web server at the same time
// because it has only 2 cores and 320KB of RAM which is not sufficient apparently (it is an Espressif ESP32-WROOM-32 38-pin module)
// so im doing what i can with what i have

SemaphoreHandle_t xMutex;

void setup() {
    Serial.begin(115200);
    initDisplay();
    displayCustomMessage("X-PERT Production\nCioc Music\nCBX the family\nAlex Botea\n");

    xMutex = xSemaphoreCreateMutex();
    if (xMutex == NULL) {
        Serial.println(F("Mutex creation failed"));
        return;
    }

    // connecting to Wi-Fi
    if (!isWiFiConnected()) {
        connectToWiFiNetwork("SSID", "PASS"); // replace with your SSID and password
    }

    if (isWiFiConnected()) {
        Serial.printf_P(PSTR("\nConnected to Wi-Fi\nClick here to open the web interface: http://%s\n\n"), WiFi.localIP().toString().c_str());
    } else {
        Serial.println(F("Failed to connect to Wi-Fi\nStarting in AP mode"));
        startAP(DEVICE_NAME, "1234"); // default: ssid is the device name and password is 1234
        printAPInfo();
        Serial.printf_P(PSTR("Access point started\nClick here to open the web interface: http://%s\n"), WiFi.softAPIP().toString().c_str());
    }

    if (isWiFiConnected()) {
        startWebServerTask(); // start the web server task
    } else {
        Serial.println(F("Web server not started\nWi-Fi not connected or AP mode not started"));
    }

    // it starts the bluetooth speaker automatically when the ESP32 is powered on
    // this thing can be controlled from the web interface
    initializeBluetoothSpeaker(); // core 1, prio 2
}

void loop() {

}