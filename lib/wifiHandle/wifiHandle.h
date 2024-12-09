#ifndef WIFIHANDLE_H
#define WIFIHANDLE_H

void showAvailableWiFiNetworks();
void saveWifiCredentials(const char *ssid, const char *password);
void loadWifiCredentials(char *ssid, char *password);
void deleteWifiCredentials();
void connectToSavedWifi();
void connectToWifi(const char *ssid, const char *password);

#endif