#ifndef WIFIHANDLE_H
#define WIFIHANDLE_H

#include <Arduino.h>
#include <WiFi.h>
#include <cstring>

#define MAX_NETWORKS 10 // maximum number of networks to store

struct WiFiCredentials {
    int ID;
    char ssid[32];
    char password[64];
};

class WiFiHandle {
public:
    WiFiHandle();
    ~WiFiHandle() = default;

    void scanNetworks(); // scan for available Wi-Fi networks
    void connect(const char* ssid, const char* password); // connect to a Wi-Fi network
    void connectToSaved(); // connect to a saved Wi-Fi network
    void connectByID(int id); // connect to a Wi-Fi network by ID
    void addCredentials(const char* ssid, const char* password); // add Wi-Fi credentials to the array
    void removeCredentials(const char* ssid); // remove Wi-Fi credentials from the array
    void listSavedCredentials(); // list all saved Wi-Fi credentials
    void clearCredentials(); // clear all saved Wi-Fi credentials
    bool isConnected() const;

    // getters
    void getIPAddress(char* ipBuffer, size_t bufferSize) const;
    void getMACAddress(char* macBuffer, size_t bufferSize) const;
    void getSSID(char* ssidBuffer, size_t bufferSize) const;
    void getRSSI(char* rssiBuffer, size_t bufferSize) const;
    void getGatewayIP(char* ipBuffer, size_t bufferSize) const;

private:
    WiFiCredentials savedNetworks[MAX_NETWORKS];
    int savedNetworksCount;
    int nextID;

    void printNetworkInfo(int index) const;
};

#endif