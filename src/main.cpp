#include <Arduino.h>
#include "displayHandle.h"
#include "radioHandle.h"
#include "wifiHandle.h"
#include "bluetooth.h"

DisplayHandle displayHandle;
WiFiHandle wifiHandle;
RadioHandle radio;
BluetoothHandle bluetooth;

void setup() {
    Serial.begin(115200);
    // Display testing
    displayHandle.initDisplay();
    displayHandle.printCustomMessage("xpert production, cbx the family");

    // Wi-Fi testing
    wifiHandle.scanNetworks();
    wifiHandle.connect("ssid", "password");

    if (wifiHandle.isConnected()) {
        char ipAddress[16]; // buffer for IP address
        wifiHandle.getIPAddress(ipAddress, sizeof(ipAddress)); // get IP address
        Serial.print("IP Address: ");
        Serial.println(ipAddress);
    } else {
        Serial.println("Wi-Fi not connected.");
    }

    // radio testing
    radio.initRadio();
    radio.tuneRadio(90.1);
    Serial.print("Current Station: ");
    Serial.println(radio.getCurrentStation());
    Serial.print("Current Frequency: ");
    Serial.println(radio.getFrequency());

    // Bluetooth testing
    bluetooth.initializeBluetoothSpeaker();
    if (bluetooth.isConnected()) {
        Serial.println("Bluetooth connected.");
    } else {
        Serial.println("Bluetooth not connected.");
    }
}

void loop() {
    
}