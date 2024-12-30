#ifndef WEB_SERVER_HANDLE_H
#define WEB_SERVER_HANDLE_H

#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <FS.h>
#include <AsyncTCP.h>

class WebServerHandle {
public:
    WebServerHandle();
    ~WebServerHandle() = default;
    
    void initSPIFFS();
    void setupWebServer();

private:
    AsyncWebServer server;
};

#endif