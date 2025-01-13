#include "webserverHandle.h"

AsyncWebServer server(80);
void initSPIFFS() {
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

void setupWebServer() {
    initSPIFFS();
    Serial.println(F("Setting up web server..."));
    server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html").setCacheControl("max-age=3600");

    if (!SPIFFS.exists("/index.html")) {
        Serial.println(F("index.html doesn't exist"));
        Serial.println(F("You can use the following command to upload the file to SPIFFS:"));
        Serial.println(F("pio run -t uploadfs"));
    } else {
        Serial.println(F("index.html exists and will be served"));
    }

    static bool radioEnabled = false;
    static bool bluetoothEnabled = false;

    server.on("/toggleRadio", HTTP_GET, [](AsyncWebServerRequest *request){
        radioEnabled = !radioEnabled;
        if (radioEnabled) {
            if (bluetoothEnabled) {
                disableBluetoothSpeaker();
                bluetoothEnabled = false;
                Serial.println(F("Bluetooth disabled"));
            }
            enableRadio();
            request->send(200, "text/plain", "Radio enabled");
            Serial.println(F("Radio enabled"));
        } else {
            disableRadio();
            request->send(200, "text/plain", "Radio disabled");
            Serial.println(F("Radio disabled"));
        }
    });

    server.on("/toggleBluetooth", HTTP_GET, [](AsyncWebServerRequest *request){
        bluetoothEnabled = !bluetoothEnabled;
        if (bluetoothEnabled) {
            if (radioEnabled) {
                disableRadio();
                radioEnabled = false;
                Serial.println(F("Radio disabled"));
            }
            Serial.println(F("Enabling Bluetooth..."));
            initializeBluetoothSpeaker();
            request->send(200, "text/plain", "Bluetooth enabled");
            Serial.println(F("Bluetooth enabled"));
        } else {
            Serial.println(F("Disabling Bluetooth..."));
            deinitializeBluetoothSpeaker();
            request->send(200, "text/plain", "Bluetooth disabled");
            Serial.println(F("Bluetooth disabled"));
        }
    });

    server.on("/increaseFrequency", HTTP_GET, [](AsyncWebServerRequest *request){
        increaseRadioFrequency();
        request->send(200, "text/plain", "Frequency increased");
    });

    server.on("/decreaseFrequency", HTTP_GET, [](AsyncWebServerRequest *request){
        decreaseRadioFrequency();
        request->send(200, "text/plain", "Frequency decreased");
    });

    server.on("/setFrequency", HTTP_GET, [](AsyncWebServerRequest *request){
        if (request->hasParam("freq")) {
            String freq = request->getParam("freq")->value();
            setRadioFrequency(freq.toFloat());
            request->send(200, "text/plain", "Frequency set to " + freq);
        } else {
            request->send(400, "text/plain", "Frequency parameter missing");
        }
    });

    server.on("/getFrequency", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/plain", String(getFrequency()));
    });

    server.on("/playNextTrack", HTTP_GET, [](AsyncWebServerRequest *request){
        playNextTrack();
        request->send(200, "text/plain", "Next track");
    });

    server.on("/playPreviousTrack", HTTP_GET, [](AsyncWebServerRequest *request){
        playPreviousTrack();
        request->send(200, "text/plain", "Previous track");
    });

    server.on("/togglePlayback", HTTP_GET, [](AsyncWebServerRequest *request){
        togglePlayback();
        request->send(200, "text/plain", "Playback toggled");
    });

    server.on("/volumeUp", HTTP_GET, [](AsyncWebServerRequest *request){
        volumeUp();
        request->send(200, "text/plain", "Volume increased");
    });

    server.on("/volumeDown", HTTP_GET, [](AsyncWebServerRequest *request){
        volumeDown();
        request->send(200, "text/plain", "Volume decreased");
    });

    server.on("/getSignalLevel", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/plain", String(getSignalLevel()));
    });

    server.on("/restart", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/plain", "Restarting...");
        delay(1000); // 1 second delay
        restartESP();
    });

    server.on("/getSystemStats", HTTP_GET, [](AsyncWebServerRequest *request){
        multi_heap_info_t heapInfo;
        heap_caps_get_info(&heapInfo, MALLOC_CAP_DEFAULT);

        size_t totalHeap = ESP.getHeapSize();
        size_t freeHeap = heapInfo.total_free_bytes;
        size_t usedHeap = totalHeap - freeHeap;
        size_t freeSPIFFFS = SPIFFS.totalBytes() - SPIFFS.usedBytes();
        size_t freePSRAM = ESP.getFreePsram();
        float chipTemp = temperatureRead();

        String json = "{";
        json += "\"totalHeap\":" + String(totalHeap / 1024) + ",";
        json += "\"freeHeap\":" + String(freeHeap / 1024) + ",";
        json += "\"usedHeap\":" + String(usedHeap / 1024) + ",";
        json += "\"freePSRAM\":" + String(freePSRAM / 1024) + ",";
        json += "\"freeSPIFFS\":" + String(freeSPIFFFS / 1024) + ",";
        json += "\"chipTemp\":" + String(chipTemp);
        json += "}";

        request->send(200, "application/json", json);
    });

    server.begin();
    Serial.println(F("Web server started"));
}