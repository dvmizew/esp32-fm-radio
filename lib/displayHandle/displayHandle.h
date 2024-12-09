#ifndef DISPLAY_H
#define DISPLAY_H

void initDisplay();
void printWiFiNetworks(); // get available WiFi networks from wifiHandle
void printWiFiConnectionStatus(); // get WiFi connection status from wifiHandle
void printWeatherInfo();
void printRDSInfo();
void printBluetoothConnectionStatus(); // get Bluetooth connection status from bluetooth
void printCustomMessage(const char *message);

#endif