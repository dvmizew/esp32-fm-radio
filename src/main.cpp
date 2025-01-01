// #include <Arduino.h>
#include "displayHandle.h"
// #include "radioHandle.h"
// #include "wifiHandle.h"
// #include "bluetooth.h"
#include "webserverHandle.h"
// #include "spotify.h"

// global objects for testing
DisplayHandle displayHandle;
// JoystickHandle joystick;
// SpotifyHandle spotify; // doesn't work yet

void setup() {
    Serial.begin(115200);

    // printSystemInfo();
    // heapCalculator();

    // Display testing
    displayHandle.initDisplay();
    // displayHandle.displayCustomMessage("X-PERT Production\nCioc Music\nCBX the family\nAlex Botea\n"); // Splash screen
    // delay(2000);
    // displayHandle.printWiFiNetworks();
    // displayHandle.printSystemInfo();

    // Wi-Fi testing
    // STATION MODE

    // connectToWiFiNetwork("ssid", "password");
    // printWiFiConnectionDetails();

    // displayHandle.printDateTime();

    // radio testing
    // initRadio();
    // searchRadioStations();
    // printRadioStations();
    // Serial.printf_P(PSTR("Current station: %s\n"), getCurrentStation());
    // joystick.testJoystick();
    // handleRadioControl(); // THIS FUNCTION DOESN'T WORK YET
    // blinkInternalLED();

    // Bluetooth testing
    initializeBluetoothSpeaker();

    // if (bluetoothIsConnected()) {
    //     Serial.println(F("Bluetooth connected."));
    //     // handleBluetoothControl();
    // }

    // Web server testing
    // initSPIFFS();
    // setupWebServer();

    displayHandle.displayResourceUsage();
    // displayHandle.displayWiFiConnectionStatus();

    // Spotify testing
    // spotify.setupSpotify();
    // it doesn't work at this point
}

void loop() {
    
}