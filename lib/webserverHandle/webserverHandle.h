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

void initSPIFFS();
void setupWebServer();

#endif