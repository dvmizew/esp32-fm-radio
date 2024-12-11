#include "displayHandle.h"

DisplayHandle::DisplayHandle()
    : I2C_display(1), // I2C bus 1
      display(128, 64, &I2C_display), // 128x64 display
      displayInitialized(false) {} // we init the display in the main program

void DisplayHandle::initDisplay() {
    I2C_display.begin(DISPLAY_SDA_PIN, DISPLAY_SCL_PIN, 400000); // start I2C bus with SDA and SCL pins
    if (!display.begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS)) {
        Serial.println(F("Error initializing display!"));
        while(true);
    }

    display.clearDisplay(); // clear display buffer
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    displayInitialized = true;
}

void DisplayHandle::printCustomMessage(const char *message) {
    clearAndUpdate();
    display.setCursor(0, 0);
    display.println(message);
    display.display();
}

void DisplayHandle::printDateTime() {
    clearAndUpdate();
    display.setCursor(0, 0);

    configTime(0, 0, "pool.ntp.org", "time.nist.gov"); // get time from NTP server

    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        display.println("Failed to obtain time\nPlease connect to Wi-Fi");
        display.display();
        return;
    }

    display.printf("Date: %02d.%02d.%04d\n", timeinfo.tm_mday, timeinfo.tm_mon + 1, timeinfo.tm_year + 1900);
    display.printf("Time: %02d:%02d:%02d\n", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);

    display.display();
}

void DisplayHandle::printSystemInfo() {
    clearAndUpdate();
    display.setCursor(0, 0);

    display.printf("CPU Freq: %d MHz\n", ESP.getCpuFreqMHz());
    display.printf("Free Heap: %d KB\n", ESP.getFreeHeap() / 1024);
    display.printf("Chip Model: %s\n", ESP.getChipModel());
    display.printf("Chip Rev: %d\n", ESP.getChipRevision());
    display.printf("Flash Size: %d MB\n", ESP.getFlashChipSize() / (1024 * 1024));

    // MAC Address
    uint8_t mac[6];
    esp_read_mac(mac, ESP_MAC_WIFI_STA);
    display.printf("MAC:%02X:%02X:%02X:%02X:%02X:%02X\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    display.display();
}

void DisplayHandle::printSPIFFSInfo() {
    clearAndUpdate();
    display.setCursor(0, 0);

    if (!SPIFFS.begin(true)) {
        display.println("Failed to mount SPIFFS");
        display.display();
        return;
    }

    display.printf("Total: %d KB\n", SPIFFS.totalBytes() / 1024);
    display.printf("Used: %d KB\n", SPIFFS.usedBytes() / 1024);

    display.display();
}

// print methods from other modules
void DisplayHandle::printWiFiNetworks() {
    Serial.println("Printing Wi-Fi networks on display...");
    printCustomMessage("Wi-Fi networks");
    WiFiHandle wifiHandle;
    int networkCount = WiFi.scanNetworks();
    clearAndUpdate();
    display.setCursor(0, 0);
    display.printf("Network count: %d\n", networkCount);
    display.display();
    delay(2000); // delay to show network count

    for (int i = 0; i < networkCount; i++) {
        clearAndUpdate();
        display.setCursor(0, 0);

        // numerotation
        display.printf("%d/%d\n", i + 1, networkCount);

        // SSID
        display.printf("%-16.16s\n", WiFi.SSID(i).c_str());

        // RSSI
        display.printf("RSSI: %4d\n", WiFi.RSSI(i));

        // channel
        display.printf("CH: %2d\n", WiFi.channel(i));

        // encryption type
        display.printf("ENC: ");
        switch (WiFi.encryptionType(i)) {
            case WIFI_AUTH_OPEN: display.println("OPEN"); break;
            case WIFI_AUTH_WEP: display.println("WEP"); break;
            case WIFI_AUTH_WPA_PSK: display.println("WPA"); break;
            case WIFI_AUTH_WPA2_PSK: display.println("WPA2"); break;
            case WIFI_AUTH_WPA_WPA2_PSK: display.println("WPA+WPA2"); break;
            case WIFI_AUTH_WPA2_ENTERPRISE: display.println("WPA2-EAP"); break;
            default: display.println("UNKNOWN");
        }

        display.display();
        delay(4000); // delay to show each network
    }
}

void DisplayHandle::printWiFiConnectionStatus() {
    clearAndUpdate();
    display.setCursor(0, 0);

    if (WiFi.status() == WL_CONNECTED) {
        display.println("Wi-Fi Connected");

        display.printf("SSID: %s\n", WiFi.SSID().c_str());

        display.printf("IP: %s\n", WiFi.localIP().toString().c_str());

        display.printf("RSSI: %d dBm\n", WiFi.RSSI());
    } else {
        display.println("Wi-Fi Not Connected");
    }

    display.display();
}

void DisplayHandle::printWeatherInfo() {
    // TODO
}

void DisplayHandle::printRDSInfo() {
    // TODO
}

void DisplayHandle::printBluetoothConnectionStatus() {
    clearAndUpdate();
    display.setCursor(0, 0);

    BluetoothHandle bluetoothHandle;

    if (bluetoothHandle.isConnected()) {
        display.println("Bluetooth Connected");
        display.printf("Device: %s\n", bluetoothHandle.getDeviceName());

        // display.printf("RSSI: %d dBm\n", bluetoothHandle.getSignalStrength());
    } else {
        display.println("Bluetooth Not Connected");
    }

    display.display();
}

// status getter for display
bool DisplayHandle::isDisplayInitialized() const {
    return displayInitialized;
}

void DisplayHandle::clearAndUpdate() {
    display.clearDisplay();
    display.setCursor(0, 0);
}