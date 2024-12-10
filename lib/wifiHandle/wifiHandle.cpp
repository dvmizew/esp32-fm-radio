#include "wifiHandle.h"
// #include <EEPROM.h>
// #include <WiFi.h>
// #include <cstring>

WiFiHandle::WiFiHandle() {
    // initialize saved credentials with empty strings
    memset(savedSSID, 0, sizeof(savedSSID));
    memset(savedPassword, 0, sizeof(savedPassword));
}

void WiFiHandle::scanNetworks() {
    Serial.println("Scanning for available Wi-Fi networks...");

    WiFi.mode(WIFI_STA); // set Wi-Fi mode to station (client)
    WiFi.disconnect(); // disconnect from any previous connections
    delay(100); // wait for disconnect to complete

    int networkCount = WiFi.scanNetworks(); // scan for available networks
    Serial.println("Scan complete!");

    if (networkCount == 0) {
        Serial.println("No networks found.");
    } else {
        Serial.printf("%d networks found:\n", networkCount);
        // print network information
        for (int i = 0; i < networkCount; i++) {
            printNetworkInfo(i);
        }
    }

    WiFi.scanDelete(); // delete scan results to free up memory
}

void WiFiHandle::printNetworkInfo(int index) const {
    Serial.printf("%2d | %-32.32s | RSSI: %4d | Channel: %2d | Encryption: ", 
                  index + 1, WiFi.SSID(index).c_str(), WiFi.RSSI(index), WiFi.channel(index));

    switch (WiFi.encryptionType(index)) {
        case WIFI_AUTH_OPEN: Serial.println("OPEN"); break;
        case WIFI_AUTH_WEP: Serial.println("WEP"); break;
        case WIFI_AUTH_WPA_PSK: Serial.println("WPA"); break;
        case WIFI_AUTH_WPA2_PSK: Serial.println("WPA2"); break;
        case WIFI_AUTH_WPA_WPA2_PSK: Serial.println("WPA+WPA2"); break;
        case WIFI_AUTH_WPA2_ENTERPRISE: Serial.println("WPA2-EAP"); break;
        default: Serial.println("UNKNOWN");
    }
}

void WiFiHandle::connect(const char* ssid, const char* password) {
    Serial.printf("Connecting to Wi-Fi: %s\n", ssid);

    WiFi.begin(ssid, password); // connect to Wi-Fi network
    unsigned long start = millis(); // start time
    const unsigned long timeout = 10000; // 10 seconds timeout

    while (WiFi.status() != WL_CONNECTED && millis() - start < timeout) {
        // connecting...
        delay(500);
        Serial.print(".");
    }

    if (WiFi.status() == WL_CONNECTED)
        Serial.println("\nConnected!");
    else
        Serial.println("\nFailed to connect.");
}

void WiFiHandle::connectToSaved() {
    // load saved credentials
    if (strlen(savedSSID) > 0)
        connect(savedSSID, savedPassword);
    else
        Serial.println("No saved Wi-Fi credentials.");
}

void WiFiHandle::saveCredentials(const char* ssid, const char* password) {
    strncpy(savedSSID, ssid, sizeof(savedSSID) - 1);
    savedSSID[sizeof(savedSSID) - 1] = '\0';

    strncpy(savedPassword, password, sizeof(savedPassword) - 1);
    savedPassword[sizeof(savedPassword) - 1] = '\0';

    EEPROM.begin(128); // begin EEPROM
    EEPROM.put(0, savedSSID); // save credentials to EEPROM
    EEPROM.put(64, savedPassword); // save credentials to EEPROM
    EEPROM.commit(); // commit changes

    Serial.println("Credentials saved.");
}

void WiFiHandle::loadCredentials(char* ssid, size_t ssidSize, char* password, size_t passwordSize) {
    EEPROM.begin(128);
    EEPROM.get(0, savedSSID);
    EEPROM.get(64, savedPassword);

    strncpy(ssid, savedSSID, ssidSize - 1);
    ssid[ssidSize - 1] = '\0';

    strncpy(password, savedPassword, passwordSize - 1);
    password[passwordSize - 1] = '\0';

    Serial.println("Credentials loaded.");
}

void WiFiHandle::deleteCredentials(const char* ssid, const char* password) {
    memset(savedSSID, 0, sizeof(savedSSID)); // clear saved credentials
    memset(savedPassword, 0, sizeof(savedPassword)); // clear saved credentials

    EEPROM.begin(128); // begin EEPROM
    EEPROM.put(0, savedSSID); // save empty credentials to EEPROM
    EEPROM.put(64, savedPassword); // save empty credentials to EEPROM
    EEPROM.commit(); // commit changes

    Serial.println("Credentials deleted.");
}

bool WiFiHandle::isConnected() const {
    return WiFi.status() == WL_CONNECTED;
}

void WiFiHandle::getIPAddress(char* ipBuffer, size_t bufferSize) const {
    if (isConnected()) {
        String ip = WiFi.localIP().toString(); // get local IP address
        strncpy(ipBuffer, ip.c_str(), bufferSize - 1); // copy IP address to buffer
        ipBuffer[bufferSize - 1] = '\0';
    } else {
        strncpy(ipBuffer, "Not connected", bufferSize - 1);
        ipBuffer[bufferSize - 1] = '\0';
    }
}

void WiFiHandle::getMACAddress(char* macBuffer, size_t bufferSize) const {
    String mac = WiFi.macAddress(); // get MAC address
    strncpy(macBuffer, mac.c_str(), bufferSize - 1);
    macBuffer[bufferSize - 1] = '\0';
}