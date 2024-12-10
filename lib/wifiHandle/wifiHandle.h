#ifndef WIFIHANDLE_H
#define WIFIHANDLE_H

#include <Arduino.h>
#include <EEPROM.h>
#include <WiFi.h>
#include <cstring>

class WiFiHandle {
public:
    WiFiHandle();

    void scanNetworks(); // scan for available Wi-Fi networks
    void connect(const char* ssid, const char* password); // connect to a Wi-Fi network
    void connectToSaved(); // connect to a saved Wi-Fi network
    void saveCredentials(const char* ssid, const char* password); // save Wi-Fi credentials to EEPROM
    void loadCredentials(char* ssid, size_t ssidSize, char* password, size_t passwordSize); // load Wi-Fi credentials from EEPROM
    void deleteCredentials(const char* ssid, const char* password); // delete saved Wi-Fi credentials from EEPROM
    bool isConnected() const;
    void getIPAddress(char* ipBuffer, size_t bufferSize) const;
    void getMACAddress(char* macBuffer, size_t bufferSize) const;

private:
    char savedSSID[33];
    char savedPassword[65];

    void printNetworkInfo(int index) const;
};

#endif