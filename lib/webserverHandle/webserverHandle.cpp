#include "webserverHandle.h"
#include "radioHandle.h"

RadioHandle radioHandle;

WebServerHandle::WebServerHandle() : server(80) {}

void WebServerHandle::initSPIFFS() {
    if (!SPIFFS.begin(true)) {
        Serial.println(F("An error has occurred while mounting SPIFFS"));
        if (SPIFFS.format()) {
            Serial.println(F("SPIFFS formatted successfully"));
        } else {
            Serial.println(F("SPIFFS format failed"));
        }
    } else {
        Serial.println(F("SPIFFS mounted successfully"));
    }
}

void WebServerHandle::setupWebServer() {
    radioHandle.initRadio();
    Serial.println(F("Setting up web server..."));
    server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");

    if (!SPIFFS.exists("/index.html")) {
        Serial.println(F("index.html doesn't exist"));
        Serial.println(F("You can use the following command to upload the file to SPIFFS:"));
        Serial.println(F("pio run -t uploadfs"));
    } else {
        Serial.println(F("index.html exists and will be served"));
    }

    server.on("/change_station", HTTP_GET, [](AsyncWebServerRequest *request){
        bool increase = request->getParam("increase")->value() == "true";
        if (increase) {
            radioHandle.increaseFrequency();
        } else {
            radioHandle.decreaseFrequency();
        }
        String response = "{\"frequency\": \"" + String(radioHandle.getFrequency()) + "\"}";
        request->send(200, "application/json", response);
    });

    server.on("/adjust_volume", HTTP_GET, [](AsyncWebServerRequest *request){
        bool increase = request->getParam("increase")->value() == "true";
        int newVolume = increase ? 6 : 4;
        String response = "{\"volume\": \"" + String(newVolume) + "\"}";
        request->send(200, "application/json", response);
    });

    server.on("/rds", HTTP_GET, [](AsyncWebServerRequest *request){
        String rdsInfo = "Example RDS Info";
        String response = "{\"info\": \"" + rdsInfo + "\"}";
        request->send(200, "application/json", response);
    });

    server.on("/weather", HTTP_GET, [](AsyncWebServerRequest *request){
        String weatherInfo = "Sunny, 25°C";
        String response = "{\"weather\": \"" + weatherInfo + "\"}";
        request->send(200, "application/json", response);
    });

    server.on("/connect", HTTP_GET, [](AsyncWebServerRequest *request){
        String message = "Connected to Internet Radio";
        String response = "{\"message\": \"" + message + "\"}";
        request->send(200, "application/json", response);
    });

    server.on("/get_signal_strength", HTTP_GET, [](AsyncWebServerRequest *request){
        short signalStrength = radioHandle.getSignalLevel();
        String response = "{\"signalStrength\": \"" + String(signalStrength) + "\"}";
        request->send(200, "application/json", response);
    });

    server.on("/set_frequency", HTTP_GET, [](AsyncWebServerRequest *request){
        String freqParam = request->getParam("frequency")->value();
        float frequency = freqParam.toFloat();
        radioHandle.setFrequency(frequency);
        String response = "{\"frequency\": \"" + String(radioHandle.getFrequency()) + "\"}";
        request->send(200, "application/json", response);
    });

    server.onNotFound([](AsyncWebServerRequest *request){
        request->send(404, "text/plain", "Not found");
    });

    server.begin();
}