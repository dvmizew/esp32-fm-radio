#include "webserverHandle.h"

AsyncWebServer server(80);
TaskHandle_t webServerTaskHandle = nullptr;

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

void setupWebServerTask(void *parameter) {
    setupWebServer();
    vTaskDelete(NULL);
}

void startWebServerTask() {
    xTaskCreatePinnedToCore(
        setupWebServerTask,
        "WebServerTask",
        WEBSERVER_TASK_HEAP_SIZE,
        nullptr,
        WEBSERVER_TASK_PRIORITY,
        &webServerTaskHandle,
        0 // assign to core 0
    );
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

    // radio endpoints
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
    
    server.on("/getSignalLevel", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/plain", String(getSignalLevel()));
    });

    // bluetooth control endpoints
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
    
    // system endpoints
    server.on("/restart", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/plain", "Restarting...");
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

    // Wi-Fi endpoints
    server.on("/getWiFiDetails", HTTP_GET, [](AsyncWebServerRequest *request){
        String json = "{";
        json += "\"ssid\":\"" + WiFi.SSID() + "\",";
        json += "\"ip\":\"" + WiFi.localIP().toString() + "\",";
        json += "\"signalStrength\":\"" + String(map(WiFi.RSSI(), -100, -40, 0, 100)) + "\"";
        json += "}";
        request->send(200, "application/json", json);
    });

    server.on("/getWiFiStatus", HTTP_GET, [](AsyncWebServerRequest *request){
        String json = "{";
        if (isWiFiConnected()) {
            json += "\"connected\":true,";
            json += "\"ssid\":\"" + WiFi.SSID() + "\",";
            json += "\"ip\":\"" + WiFi.localIP().toString() + "\",";
            json += "\"signalStrength\":\"" + String(map(WiFi.RSSI(), -100, -40, 0, 100)) + "\"";
        } else {
            json += "\"connected\":false";
        }
        json += "}";
        request->send(200, "application/json", json);
    });

    server.on("/scanWiFiNetworks", HTTP_GET, [](AsyncWebServerRequest *request){
        int n = WiFi.scanNetworks();
        String json = "[";
        for (int i = 0; i < n; ++i) {
            if (i > 0) json += ",";
            json += "{";
            json += "\"ssid\":\"" + WiFi.SSID(i) + "\",";
            json += "\"rssi\":" + String(WiFi.RSSI(i));
            json += "}";
        }
        json += "]";
        request->send(200, "application/json", json);
    });

    server.on("/connectToWiFi", HTTP_GET, [](AsyncWebServerRequest *request){
        if (request->hasParam("ssid") && request->hasParam("password")) {
            String ssid = request->getParam("ssid")->value();
            String password = request->getParam("password")->value();
            if (ssid.isEmpty() || password.isEmpty()) {
                request->send(400, "text/plain", "SSID or password cannot be empty");
                return;
            }
            connectToWiFiNetwork(ssid.c_str(), password.c_str());
            request->send(200, "text/plain", "Connecting to Wi-Fi...");
        } else {
            request->send(400, "text/plain", "SSID or password parameter missing");
        }
    });

    server.on("/addWiFiCredentials", HTTP_GET, [](AsyncWebServerRequest *request){
        if (request->hasParam("ssid") && request->hasParam("password")) {
            String ssid = request->getParam("ssid")->value();
            String password = request->getParam("password")->value();
            if (ssid.isEmpty() || password.isEmpty()) {
                request->send(400, "text/plain", "SSID or password cannot be empty");
                return;
            }
            addWiFiCredentials(savedNetworks, &savedNetworksCount, &nextID, ssid.c_str(), password.c_str());
            request->send(200, "text/plain", "Wi-Fi credentials added");
        } else {
            request->send(400, "text/plain", "SSID or password parameter missing");
        }
    });

    server.on("/printWiFiConnectionStatus", HTTP_GET, [](AsyncWebServerRequest *request){
        String status;
        if (WiFi.status() == WL_CONNECTED) {
            status = "Connected to " + WiFi.SSID() + "\n";
            status += "IP address: " + WiFi.localIP().toString() + "\n";
            status += "RSSI: " + String(WiFi.RSSI()) + " dBm\n";
            status += "Signal strength: " + String(map(WiFi.RSSI(), -100, -40, 0, 100)) + "/100\n";
        } else {
            status = "Wi-Fi not connected.";
        }
        request->send(200, "text/plain", status);
    });

    server.on("/removeWiFiCredentials", HTTP_GET, [](AsyncWebServerRequest *request){
        if (request->hasParam("ssid")) {
            String ssid = request->getParam("ssid")->value();
            removeWiFiCredentials(savedNetworks, &savedNetworksCount, ssid.c_str());
            request->send(200, "text/plain", "Wi-Fi credentials removed");
        } else {
            request->send(400, "text/plain", "SSID parameter missing");
        }
    });

    server.on("/clearWiFiCredentials", HTTP_GET, [](AsyncWebServerRequest *request){
        clearAllWiFiCredentials(savedNetworks, &savedNetworksCount, &nextID);
        request->send(200, "text/plain", "All Wi-Fi credentials cleared");
    });

    server.on("/getSavedWiFiCredentials", HTTP_GET, [](AsyncWebServerRequest *request){
        String json = "[";
        for (int i = 0; i < savedNetworksCount; ++i) {
            if (i > 0) json += ",";
            json += "{";
            json += "\"id\":" + String(savedNetworks[i].ID) + ",";
            json += "\"ssid\":\"" + String(savedNetworks[i].ssid) + "\"";
            json += "}";
        }
        json += "]";
        request->send(200, "application/json", json);
    });

    server.on("/startAP", HTTP_GET, [](AsyncWebServerRequest *request){
        if (request->hasParam("ssid") && request->hasParam("password")) {
            String ssid = request->getParam("ssid")->value();
            String password = request->getParam("password")->value();
            if (ssid.isEmpty() || password.isEmpty()) {
                request->send(400, "text/plain", "SSID or password cannot be empty");
                return;
            }
            startAP(ssid.c_str(), password.c_str());
            request->send(200, "text/plain", "Access point started");
        } else {
            request->send(400, "text/plain", "SSID or password parameter missing");
        }
    });

    server.on("/stopAP", HTTP_GET, [](AsyncWebServerRequest *request){
        stopAP();
        request->send(200, "text/plain", "Access point stopped");
    });

    server.on("/printAPInfo", HTTP_GET, [](AsyncWebServerRequest *request){
        String info;
        info += "AP SSID: " + WiFi.softAPSSID() + "\n";
        info += "AP IP: " + WiFi.softAPIP().toString() + "\n";
        request->send(200, "text/plain", info);
    });

    server.on("/printConnectedDevices", HTTP_GET, [](AsyncWebServerRequest *request){
        printConnectedDevices();
        request->send(200, "text/plain", "Connected devices printed to Serial");
    });

    server.on("/getDevicesCount", HTTP_GET, [](AsyncWebServerRequest *request){
        int count = getDevicesCount();
        request->send(200, "text/plain", String(count));
    });

    server.begin();
    Serial.println(F("Web server started"));
}