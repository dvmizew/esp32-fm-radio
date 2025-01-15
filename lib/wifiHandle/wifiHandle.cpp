#include "wifiHandle.h"

WiFiCredentials savedNetworks[MAX_NETWORKS];
uint8_t savedNetworksCount = 0;
uint8_t nextID = 1;

// timer functions
volatile unsigned long timer1_millis = 0;
hw_timer_t *timer1 = NULL;
hw_timer_t *timer2 = NULL;

void IRAM_ATTR onTimer1() {
    timer1_millis++;
}

void IRAM_ATTR onTimer2() {
    // perform periodic Wi-Fi status check or other tasks
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println(F("Wi-Fi is connected"));
    } else {
        Serial.println(F("Wi-Fi is not connected"));
    }
}

void initTimers() {
    // initialize Timer1 for custom millis
    timer1 = timerBegin(0, 80, true); // timer 0, prescaler 80, count up
    timerAttachInterrupt(timer1, &onTimer1, true);
    timerAlarmWrite(timer1, 1000, true); // 1ms interval
    timerAlarmEnable(timer1);

    // initialize Timer2 for periodic tasks
    timer2 = timerBegin(1, 80, true); // timer 1, prescaler 80, count up
    timerAttachInterrupt(timer2, &onTimer2, true);
    timerAlarmWrite(timer2, 5000000, true); // 5 seconds interval
    timerAlarmEnable(timer2);
}

unsigned long customMillis() {
    return timer1_millis;
}

// wifi functions
void printWiFiConnectionDetails() {
    Serial.printf_P(PSTR("Connected to %s\n"), WiFi.SSID().c_str());
    Serial.printf_P(PSTR("IP address: %s\n"), WiFi.localIP().toString().c_str());
    Serial.printf_P(PSTR("RSSI: %d dBm\n"), WiFi.RSSI());
    Serial.printf_P(PSTR("Signal strength: %d/100\n"), map(WiFi.RSSI(), -100, -40, 0, 100));
}

// STATION MODE
void scanWiFiNetworks() {
    int n = WiFi.scanNetworks();
    Serial.printf_P(PSTR("Number of networks found: %d\n"), n);
    for (int i = 0; i < n; ++i) {
        Serial.printf_P(PSTR("%d: %s (%d)\n"), i + 1, WiFi.SSID(i).c_str(), WiFi.RSSI(i));
    }
}

void connectToWiFiNetwork(const char* ssid, const char* password) {
    Serial.printf_P(PSTR("Connecting to Wi-Fi: %s\n"), ssid);
    WiFi.begin(ssid, password);
    unsigned long start = customMillis();
    const unsigned long timeout = 10000; // 10 seconds timeout

    while (WiFi.status() != WL_CONNECTED && customMillis() - start < timeout) {
        delay(500);
        Serial.print(F("."));
    }
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println(F("\nConnected!"));
    } else {
        Serial.println(F("\nFailed to connect."));
    }
}

void printWiFiConnectionStatus() {
    if (WiFi.status() == WL_CONNECTED) {
        printWiFiConnectionDetails();
    } else {
        Serial.println(F("Wi-Fi not connected."));
    }
}

void addWiFiCredentials(WiFiCredentials* savedNetworks, uint8_t* savedNetworksCount, uint8_t* nextID, const char* ssid, const char* password) {
    if (*savedNetworksCount < MAX_NETWORKS) {
        savedNetworks[*savedNetworksCount].ID = (*nextID)++;
        strncpy(savedNetworks[*savedNetworksCount].ssid, ssid, MAX_SSID_LEN);
        strncpy(savedNetworks[*savedNetworksCount].password, password, MAX_PASS_LEN);
        (*savedNetworksCount)++;
        Serial.println(F("Credentials added."));
    } else {
        Serial.println(F("Max networks reached."));
    }
}

void removeWiFiCredentials(WiFiCredentials* savedNetworks, uint8_t* savedNetworksCount, const char* ssid) {
    bool found = false;
    for (int i = 0; i < *savedNetworksCount; ++i) {
        if (strncmp(savedNetworks[i].ssid, ssid, MAX_SSID_LEN) == 0) {
            found = true;
            for (int j = i; j < *savedNetworksCount - 1; ++j) {
                savedNetworks[j] = savedNetworks[j + 1];
            }
            (*savedNetworksCount)--;
            Serial.println(F("Credentials removed."));
            break;
        }
    }
    if (!found) {
        Serial.println(F("SSID not found."));
    }
}

void printSavedWiFiCredentials(const WiFiCredentials* savedNetworks, uint8_t savedNetworksCount) {
    Serial.printf_P(PSTR("Number of saved networks: %d\n"), savedNetworksCount);
    for (int i = 0; i < savedNetworksCount; ++i) {
        Serial.printf_P(PSTR("ID: %d, SSID: %s\n"), savedNetworks[i].ID, savedNetworks[i].ssid);
    }
}

void clearAllWiFiCredentials(WiFiCredentials* savedNetworks, uint8_t* savedNetworksCount, uint8_t* nextID) {
    *savedNetworksCount = 0;
    *nextID = 1;
    Serial.println(F("All credentials cleared."));
}

bool isWiFiConnected() {
    return WiFi.status() == WL_CONNECTED;
}

// AP MODE
void startAP(const char* ssid, const char* password) {
    WiFi.mode(WIFI_AP);
    if (WiFi.softAP(ssid, password)) {
        Serial.println(F("Access point started."));
        printAPInfo();
    } else {
        Serial.println(F("Failed to start access point."));
    }
}

void stopAP() {
    if (WiFi.softAPdisconnect(true)) {
        Serial.println(F("Access point stopped."));
    } else {
        Serial.println(F("Failed to stop access point."));
    }
}

void printAPInfo() {
    Serial.printf_P(PSTR("AP SSID: %s\n"), WiFi.softAPSSID().c_str());
    Serial.printf_P(PSTR("AP IP: %s\n"), WiFi.softAPIP().toString().c_str());
}

void printConnectedDevices() {
    wifi_sta_list_t wifi_sta_list;
    tcpip_adapter_sta_list_t adapter_sta_list;

    if (esp_wifi_ap_get_sta_list(&wifi_sta_list) == ESP_OK && tcpip_adapter_get_sta_list(&wifi_sta_list, &adapter_sta_list) == ESP_OK) {
        Serial.printf_P(PSTR("Number of connected devices: %d\n"), adapter_sta_list.num);
        for (int i = 0; i < adapter_sta_list.num; i++) {
            tcpip_adapter_sta_info_t station = adapter_sta_list.sta[i];
            Serial.printf_P(PSTR("Device %d: MAC: %02X:%02X:%02X:%02X:%02X:%02X, IP: %s\n"), i + 1,
                            station.mac[0], station.mac[1], station.mac[2],
                            station.mac[3], station.mac[4], station.mac[5],
                            ip4addr_ntoa((const ip4_addr_t*)&station.ip));
        }
    } else {
        Serial.println(F("Failed to get connected devices list."));
    }
}

int getDevicesCount() {
    wifi_sta_list_t wifi_sta_list;
    tcpip_adapter_sta_list_t adapter_sta_list;

    if (esp_wifi_ap_get_sta_list(&wifi_sta_list) == ESP_OK && tcpip_adapter_get_sta_list(&wifi_sta_list, &adapter_sta_list) == ESP_OK) {
        return adapter_sta_list.num;
    } else {
        Serial.println(F("Failed to get connected devices count."));
        return 0;
    }
}