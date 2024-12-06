#include "webserver.h"

AsyncWebServer server(80);

void setupWebServer() {
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", "ESP32 FM Radio Web Interface");
    });
    server.begin();
}