#ifndef DISPLAY_H
#define DISPLAY_H

#define SSD1306_I2C_ADDRESS 0x3C

void initDisplay();
void updateDisplay();
void printWiFiNetworks();
void printWiFiConnectionStatus();
void printWeatherInfo();
void printRDSInfo();
void printBluetoothConnectionStatus();

#endif