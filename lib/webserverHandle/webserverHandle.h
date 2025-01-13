#ifndef WEB_SERVER_HANDLE_H
#define WEB_SERVER_HANDLE_H

#include <ESPAsyncWebServer.h> // for web server (main library)
#include <SPIFFS.h> // for saving the web interface files
#include <FS.h> // file system
#include <AsyncTCP.h> // for TCP connection
#include "wifiHandle.h" // for network connection
#include "radioHandle.h" // for radio control
#include "bluetooth.h" // for Bluetooth control

extern AsyncWebServer server;
extern TaskHandle_t webServerTaskHandle;

void initSPIFFS();
void setupWebServer();
void setupWebServerTask(void *parameter); // task function for setting up the web server
void startWebServerTask(); // function to start the web server task

#endif