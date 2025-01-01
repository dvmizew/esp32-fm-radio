#include "wifiHandle.h"

void printWiFiConnectionDetails() {
    Serial.printf_P(PSTR("Connected to %s\n"), WiFi.SSID().c_str());
    Serial.printf_P(PSTR("IP address: %s\n"), WiFi.localIP().toString().c_str());
    Serial.printf_P(PSTR("RSSI: %d dBm\n"), WiFi.RSSI());
    Serial.printf_P(PSTR("Signal strength: %d/100\n"), map(WiFi.RSSI(), -100, -40, 0, 100));
}

// STATION MODE
void scanWiFiNetworks() {
    int n = WiFi.scanNetworks();
    Serial.printf_P(PSTR("Number of networks found: %d\n"), n);
    for (int i = 0; i < n; ++i) {
        Serial.printf_P(PSTR("%d: %s (%d)\n"), i + 1, WiFi.SSID(i).c_str(), WiFi.RSSI(i));
    }
}

void connectToWiFiNetwork(const char* ssid, const char* password) {
    Serial.printf_P(PSTR("Connecting to Wi-Fi: %s\n"), ssid);
    WiFi.begin(ssid, password);
    unsigned long start = millis();
    const unsigned long timeout = 10000; // 10 seconds timeout

    while (WiFi.status() != WL_CONNECTED && millis() - start < timeout) {
        delay(500);
        Serial.print(F("."));
    }
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println(F("\nConnected!"));
    } else {
        Serial.println(F("\nFailed to connect."));
    }
}

void printWiFiConnectionStatus() {
    if (WiFi.status() == WL_CONNECTED) {
        printWiFiConnectionDetails();
    } else {
        Serial.println(F("Wi-Fi not connected."));
    }
}

void addWiFiCredentials(WiFiCredentials* savedNetworks, uint8_t* savedNetworksCount, uint8_t* nextID, const char* ssid, const char* password) {
    if (*savedNetworksCount < MAX_NETWORKS) {
        savedNetworks[*savedNetworksCount].ID = (*nextID)++;
        strncpy(savedNetworks[*savedNetworksCount].ssid, ssid, MAX_SSID_LEN);
        strncpy(savedNetworks[*savedNetworksCount].password, password, MAX_PASS_LEN);
        (*savedNetworksCount)++;
        Serial.println(F("Credentials added."));
    } else {
        Serial.println(F("Max networks reached."));
    }
}

void removeWiFiCredentials(WiFiCredentials* savedNetworks, uint8_t* savedNetworksCount, const char* ssid) {
    for (int i = 0; i < *savedNetworksCount; ++i) {
        if (strncmp(savedNetworks[i].ssid, ssid, MAX_SSID_LEN) == 0) {
            for (int j = i; j < *savedNetworksCount - 1; ++j) {
                savedNetworks[j] = savedNetworks[j + 1];
            }
            (*savedNetworksCount)--;
            Serial.println(F("Credentials removed."));
            return;
        }
    }
    Serial.println(F("SSID not found."));
}

void printSavedWiFiCredentials(const WiFiCredentials* savedNetworks, uint8_t savedNetworksCount) {
    Serial.printf_P(PSTR("Number of saved networks: %d\n"), savedNetworksCount);
    for (int i = 0; i < savedNetworksCount; ++i) {
        Serial.printf_P(PSTR("ID: %d, SSID: %s\n"), savedNetworks[i].ID, savedNetworks[i].ssid);
    }
}

void clearAllWiFiCredentials(WiFiCredentials* savedNetworks, uint8_t* savedNetworksCount, uint8_t* nextID) {
    *savedNetworksCount = 0;
    *nextID = 1;
    Serial.println(F("All credentials cleared."));
}

bool isWiFiConnected() {
    return WiFi.status() == WL_CONNECTED;
}

// AP MODE
void startAP(const char* ssid, const char* password) {
    WiFi.softAP(ssid, password);
    Serial.println(F("Access point started."));
}

void stopAP() {
    WiFi.softAPdisconnect();
    Serial.println(F("Access point stopped."));
}

void printAPInfo() {
    Serial.printf_P(PSTR("AP SSID: %s\n"), WiFi.softAPSSID().c_str());
    Serial.printf_P(PSTR("AP IP: %s\n"), WiFi.softAPIP().toString().c_str());
}

void printConnectedDevices() {
    // TODO
}

int getDevicesCount() {
    // TODO
    return 0;
}