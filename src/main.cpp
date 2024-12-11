#include <Arduino.h>
#include "displayHandle.h"
#include "radioHandle.h"
#include "wifiHandle.h"
#include "bluetooth.h"

DisplayHandle displayHandle;
WiFiHandle wifiHandle;
RadioHandle radio;
BluetoothHandle bluetooth;
JoystickHandle joystick;

void setup() {
    Serial.begin(115200);
    // Display testing
    displayHandle.initDisplay();
    displayHandle.printCustomMessage("X-PERT Production\nCioc Music\nCBX the family\nAlex Botea\n"); // Splash screen
    delay(2000);
    displayHandle.printWiFiNetworks();
    displayHandle.printSystemInfo();

    // Wi-Fi testing
    wifiHandle.scanNetworks();
    // wifiHandle.connect("ssid", "password");
    wifiHandle.saveCredentials("ssid", "password");
    wifiHandle.connectToSaved();

    char ipBuffer[16];
    char gatewayBuffer[16];
    char ssidBuffer[33];
    char rssiBuffer[5];
    wifiHandle.getIPAddress(ipBuffer, sizeof(ipBuffer));
    wifiHandle.getGatewayIP(gatewayBuffer, sizeof(gatewayBuffer));
    wifiHandle.getSSID(ssidBuffer, sizeof(ssidBuffer));
    wifiHandle.getRSSI(rssiBuffer, sizeof(rssiBuffer));

    Serial.print("IP Address: ");
    Serial.println(ipBuffer);

    Serial.print("Gateway IP: ");
    Serial.println(gatewayBuffer);

    Serial.print("SSID: ");
    Serial.println(ssidBuffer);

    Serial.print("RSSI: ");
    Serial.println(rssiBuffer);

    displayHandle.printDateTime();

    // // radio testing
    radio.initRadio();
    // joystick.testJoystick();
    // radio.handleRadioControl(); // THIS FUNCTION DOESN'T WORK YET

    // Bluetooth testing
    bluetooth.initializeBluetoothSpeaker();
    if (bluetooth.isConnected()) {
        Serial.println("Bluetooth connected.");
    }
}

void loop() {
    
}