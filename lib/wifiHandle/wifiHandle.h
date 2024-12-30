#ifndef WIFIHANDLE_H
#define WIFIHANDLE_H

#include <Arduino.h>
#include <WiFi.h>

#define MAX_NETWORKS 10 // maximum number of networks to store
#define MAX_SSID_LEN 32 // maximum length of SSID
#define MAX_PASS_LEN 64 // maximum length of password

struct WiFiCredentials {
    uint8_t ID;
    char ssid[MAX_SSID_LEN];
    char password[MAX_PASS_LEN];
};

// STATION MODE
void scanWiFiNetworks(); // scan for available Wi-Fi networks
void connectToWiFiNetwork(const char* ssid, const char* password); // connect to a Wi-Fi network
void printWiFiConnectionStatus(); // print Wi-Fi connection status
void addWiFiCredentials(WiFiCredentials* savedNetworks, uint8_t* savedNetworksCount, uint8_t* nextID, const char* ssid, const char* password); // add Wi-Fi credentials to the array
void removeWiFiCredentials(WiFiCredentials* savedNetworks, uint8_t* savedNetworksCount, const char* ssid); // remove Wi-Fi credentials from the array
void printSavedWiFiCredentials(const WiFiCredentials* savedNetworks, uint8_t savedNetworksCount); // list all saved Wi-Fi credentials
void clearAllWiFiCredentials(WiFiCredentials* savedNetworks, uint8_t* savedNetworksCount, uint8_t* nextID); // clear all saved Wi-Fi credentials
bool isWiFiConnected(); // check if connected to Wi-Fi

// AP MODE
void startAP(const char* ssid, const char* password); // start an access point
void stopAP(); // stop the access point
void printAPInfo(); // print access point information
void printConnectedDevices(); // print connected devices to the access point
int getDevicesCount(); // get the number of connected devices

#endif // WIFIHANDLE_H