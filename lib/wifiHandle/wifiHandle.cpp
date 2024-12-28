#include "wifiHandle.h"

WiFiHandle::WiFiHandle() : savedNetworksCount(0), nextID(1) {
    // initialize saved networks with empty strings
    for (int i = 0; i < MAX_NETWORKS; i++) {
        savedNetworks[i].ID = 0;
        memset(savedNetworks[i].ssid, 0, sizeof(savedNetworks[i].ssid));
        memset(savedNetworks[i].password, 0, sizeof(savedNetworks[i].password));
    }
    Serial.println("WiFiHandle initialized");
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
    for (int i = 0; i < savedNetworksCount; i++) {
        connect(savedNetworks[i].ssid, savedNetworks[i].password);
        if (isConnected()) {
            Serial.printf("Connected to saved network: %s\n", savedNetworks[i].ssid);
            break;
        }
    }
}

void WiFiHandle::connectByID(int id) {
    for (int i = 0; i < savedNetworksCount; i++) {
        if (savedNetworks[i].ID == id) {
            connect(savedNetworks[i].ssid, savedNetworks[i].password);
            return;
        }
    }
    Serial.println("Network ID not found.");
}

void WiFiHandle::addCredentials(const char* ssid, const char* password) {
    if (savedNetworksCount < MAX_NETWORKS) {
        savedNetworks[savedNetworksCount].ID = nextID++;
        strncpy(savedNetworks[savedNetworksCount].ssid, ssid, sizeof(savedNetworks[savedNetworksCount].ssid) - 1);
        savedNetworks[savedNetworksCount].ssid[sizeof(savedNetworks[savedNetworksCount].ssid) - 1] = '\0';

        strncpy(savedNetworks[savedNetworksCount].password, password, sizeof(savedNetworks[savedNetworksCount].password) - 1);
        savedNetworks[savedNetworksCount].password[sizeof(savedNetworks[savedNetworksCount].password) - 1] = '\0';

        savedNetworksCount++;
        Serial.println("Credentials added.");
    } else {
        Serial.println("Maximum number of networks reached.");
    }
}

void WiFiHandle::removeCredentials(const char* ssid) {
    for (int i = 0; i < savedNetworksCount; i++) {
        if (strcmp(savedNetworks[i].ssid, ssid) == 0) {
            for (int j = i; j < savedNetworksCount - 1; j++) {
                savedNetworks[j] = savedNetworks[j + 1];
            }
            savedNetworksCount--;
            Serial.println("Credentials removed.");
            return;
        }
    }
    Serial.println("SSID not found.");
}

void WiFiHandle::listSavedCredentials() {
    Serial.println("Saved Wi-Fi networks:");
    for (int i = 0; i < savedNetworksCount; i++) {
        Serial.printf("%d: %s (ID: %d)\n", i + 1, savedNetworks[i].ssid, savedNetworks[i].ID);
    }
}

void WiFiHandle::clearCredentials() {
    savedNetworksCount = 0;
    Serial.println("All credentials cleared.");
}

bool WiFiHandle::isConnected() const {
    return WiFi.status() == WL_CONNECTED;
}

// getters
void WiFiHandle::getIPAddress(char* ipBuffer, size_t bufferSize) const {
    if (isConnected()) {
        String ip = WiFi.localIP().toString(); // get local IP address
        strncpy(ipBuffer, ip.c_str(), bufferSize - 1); // copy IP address to buffer
        ipBuffer[bufferSize - 1] = '\0';
    } else {
        strncpy(ipBuffer, "Not connected", bufferSize - 1);
        ipBuffer[bufferSize - 1] = '\0';
    }
    Serial.println(ipBuffer);
}

void WiFiHandle::getGatewayIP(char* ipBuffer, size_t bufferSize) const {
    if (isConnected()) {
        String ip = WiFi.gatewayIP().toString(); // get gateway IP address
        strncpy(ipBuffer, ip.c_str(), bufferSize - 1); // copy IP address to buffer
        ipBuffer[bufferSize - 1] = '\0';
    } else {
        strncpy(ipBuffer, "Not connected", bufferSize - 1);
        ipBuffer[bufferSize - 1] = '\0';
    }
    Serial.println(ipBuffer);
}

void WiFiHandle::getMACAddress(char* macBuffer, size_t bufferSize) const {
    String mac = WiFi.macAddress(); // get MAC address
    strncpy(macBuffer, mac.c_str(), bufferSize - 1);
    macBuffer[bufferSize - 1] = '\0';
}

void WiFiHandle::getSSID(char* ssidBuffer, size_t bufferSize) const {
    if (isConnected()) {
        String ssid = WiFi.SSID(); // get SSID
        strncpy(ssidBuffer, ssid.c_str(), bufferSize - 1);
        ssidBuffer[bufferSize - 1] = '\0';
    } else {
        strncpy(ssidBuffer, "Not connected", bufferSize - 1);
        ssidBuffer[bufferSize - 1] = '\0';
    }
    Serial.println(ssidBuffer);
}

void WiFiHandle::getRSSI(char* rssiBuffer, size_t bufferSize) const {
    if (isConnected()) {
        int rssi = WiFi.RSSI(); // get signal strength (RSSI)
        snprintf(rssiBuffer, bufferSize, "%d dBm", rssi);
    } else {
        strncpy(rssiBuffer, "Not connected", bufferSize - 1);
        rssiBuffer[bufferSize - 1] = '\0';
    }
    Serial.println(rssiBuffer);
}