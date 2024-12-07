#include "SPIFFS.h"
#include "webserver.h"

AsyncWebServer server(80);

void initSPIFFS() {
    if (!SPIFFS.begin(true)) {
        Serial.println("Failed to mount SPIFFS, formatting...");
        SPIFFS.format();
        if (!SPIFFS.begin(true)) {
            Serial.println("Failed to initialize SPIFFS again");
            return;
        }
    }

    // Print SPIFFS information
    Serial.print("SPIFFS Total space: ");
    Serial.println(SPIFFS.totalBytes());
    Serial.print("SPIFFS Used space: ");
    Serial.println(SPIFFS.usedBytes());

}

void setupWebServer() {
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        File file = SPIFFS.open("/index.html", "r");
        if (!file) {
            request->send(404, "text/plain", "File not found");
            return;
        }

        String content = file.readString();
        request->send(200, "text/html", content);
        file.close();
    });

    server.on("/rds", HTTP_GET, [](AsyncWebServerRequest *request) {
        getRDSInfo(request);
    });

    server.on("/weather", HTTP_GET, [](AsyncWebServerRequest *request) {
        getWeatherInfo(request);
    });

    server.on("/connectIRS", HTTP_GET, [](AsyncWebServerRequest *request) {
        connectToInternetRadioStation(request);
    });

    server.begin();
}

void getRDSInfo(AsyncWebServerRequest *request) {
    // get RDS info from some API
    String rdsInfo = "RDS Info: Radio Station Name";
    request->send(200, "text/plain", rdsInfo);
}

void getWeatherInfo(AsyncWebServerRequest *request) {
    // get weather info from some API (OpenWeatherMap)
    String weather = "Temperature: 22°C, Condition: Sunny";
    request->send(200, "text/plain", weather);
}

void connectToInternetRadioStation(AsyncWebServerRequest *request) {
    // TODO
    String message = "Connecting to Internet Radio Station...";
    request->send(200, "text/plain", message);
}