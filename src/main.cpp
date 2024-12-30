// #include <Arduino.h>
#include "displayHandle.h"
#include "radioHandle.h"
#include "wifiHandle.h"
// #include "bluetooth.h"
#include "webserverHandle.h"
// #include "spotify.h"

// global objects for testing
DisplayHandle displayHandle;
// WiFiHandle wifiHandle;
RadioHandle radio;
// BluetoothHandle bluetooth;
// JoystickHandle joystick;
// SpotifyHandle spotify; // doesn't work yet

// void heapCalculator() {
//     int totalMemoryUsed = sizeof(displayHandle) + sizeof(wifiHandle) + sizeof(radio) + sizeof(bluetooth) + sizeof(joystick) + sizeof(spotify);

//     Serial.print(F("Total memory used: "));
//     Serial.print(totalMemoryUsed / 1024);
//     Serial.println(F(" KB"));

//     Serial.printf("DisplayHandle: %d KB\n", sizeof(displayHandle) / 1024);
//     Serial.printf("WiFiHandle: %d KB\n", sizeof(wifiHandle) / 1024);
//     Serial.printf("RadioHandle: %d KB\n", sizeof(radio) / 1024);
//     Serial.printf("BluetoothHandle: %d KB\n", sizeof(bluetooth) / 1024);
//     Serial.printf("JoystickHandle: %d KB\n", sizeof(joystick) / 1024);
//     Serial.printf("SpotifyHandle: %d KB\n", sizeof(spotify) / 1024);
// }

void setup() {
    Serial.begin(115200);

    // printSystemInfo();
    // heapCalculator();

    // Display testing
    displayHandle.initDisplay();
    displayHandle.displayCustomMessage("X-PERT Production\nCioc Music\nCBX the family\nAlex Botea\n"); // Splash screen
    // delay(2000);
    // displayHandle.printWiFiNetworks();
    // displayHandle.printSystemInfo();

    // Wi-Fi testing
    // STATION MODE

    connectToWiFiNetwork("ssid", "password");
    printWiFiConnectionStatus();

    // wifiHandle.scanNetworks();
    // wifiHandle.connect("ssid", "password");
    // wifiHandle.addCredentials("ssid", "password");
    // wifiHandle.connectByID(1);

    // if (wifiHandle.isConnected()) {
    //     Serial.println(F("Connected to Wi-Fi"));

    //     char ipBuffer[16];
    //     char gatewayBuffer[16];
    //     char ssidBuffer[33];
    //     char rssiBuffer[5];
    //     wifiHandle.getIPAddress(ipBuffer, sizeof(ipBuffer));
    //     wifiHandle.getGatewayIP(gatewayBuffer, sizeof(gatewayBuffer));
    //     wifiHandle.getSSID(ssidBuffer, sizeof(ssidBuffer));
    //     wifiHandle.getRSSI(rssiBuffer, sizeof(rssiBuffer));

    //     Serial.print(F("IP Address: "));
    //     Serial.println(ipBuffer);

    //     Serial.print(F("Gateway IP: "));
    //     Serial.println(gatewayBuffer);

    //     Serial.print(F("SSID: "));
    //     Serial.println(ssidBuffer);

    //     Serial.print(F("RSSI: "));
    //     Serial.println(rssiBuffer);
    // }

    // AP MODE
    // wifiHandle.startAP(DEVICE_NAME, "hehexd");
    // wifiHandle.getDevicesCount();

    // displayHandle.printDateTime();

    // radio testing
    // radio.initRadio();
    // radio.searchRadioStations();
    // radio.printRadioStations();
    // Serial.println(radio.getCurrentStation());
    // joystick.testJoystick();
    // radio.handleRadioControl(); // THIS FUNCTION DOESN'T WORK YET
    // blinkInternalLED();

    // Bluetooth testing
    // bluetooth.initializeBluetoothSpeaker();
    // if (bluetooth.isConnected()) {
    //     Serial.println(F("Bluetooth connected."));
    //     // bluetooth.handleBluetoothControl();
    // }

    // Web server testing
    WebServerHandle webserver;
    webserver.initSPIFFS();
    webserver.setupWebServer();

    displayHandle.displayResourceUsage();
    // displayHandle.displayWiFiConnectionStatus();

    // Spotify testing
    // spotify.setupSpotify();
    // it doesn't work at this point
}

void loop() {
    
}