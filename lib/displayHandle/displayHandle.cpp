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
    if (strlen(message) > 16) {
        display.setTextSize(1);
    } else {
       display.setTextSize(2);
    }
    display.println(message);
    display.display();
}

void DisplayHandle::printDateTime() {
    while (true) {
        clearAndUpdate();
        display.setCursor(0, 0);
        
        configTime(0, 0, "pool.ntp.org", "time.nist.gov"); // get time from NTP server

        struct tm timeinfo;
        if (!getLocalTime(&timeinfo)) {
            display.println("Failed to obtain time\nPlease connect to \nWi-Fi");
            display.display();
            vTaskDelay(1000 / portTICK_PERIOD_MS); // 1 second delay
            continue;
        }

        display.printf("Date: %02d.%02d.%04d\n", timeinfo.tm_mday, timeinfo.tm_mon + 1, timeinfo.tm_year + 1900);
        display.printf("Time: %02d:%02d:%02d\n", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);

        display.display();
        vTaskDelay(1000 / portTICK_PERIOD_MS); // 1 second delay
    }
}

void DisplayHandle::printDateTimeTask(void *pvParameters) {
    DisplayHandle *displayHandle = static_cast<DisplayHandle*>(pvParameters);
    displayHandle->printDateTime();
    vTaskDelete(NULL);
}

void DisplayHandle::startPrintDateTimeTask() {
    xTaskCreate(&DisplayHandle::printDateTimeTask, "DateTimeTask", 4096, this, 1, NULL);
}

void DisplayHandle::printResourceUsage() {
    // this function locks the device and doesn't allow other functions to run
    clearAndUpdate();
    display.setCursor(0, 0);

    while (true) {
        display.clearDisplay();
        display.setCursor(0, 0);
        display.printf("RESOURCE USAGE\n\n");
        display.printf("Free Heap: %d KB\n", ESP.getFreeHeap() / 1024);
        if (ESP.getFreePsram() > 0) // check if PSRAM is available
            display.printf("Free PSRAM: %d KB\n", ESP.getFreePsram() / 1024);
        display.printf("CPU Freq: %d MHz\n", ESP.getCpuFreqMHz());
        display.printf("Chip Temp: %0.2f C\n", (double)temperatureRead());
        display.display();
        vTaskDelay(1000 / portTICK_PERIOD_MS); // 1 second delay
    }
}

void DisplayHandle::printResourceUsageTask(void *pvParameters) {
    DisplayHandle *displayHandle = static_cast<DisplayHandle*>(pvParameters);
    displayHandle->printResourceUsage();
    vTaskDelete(NULL);
}

void DisplayHandle::startPrintResourceUsageTask() {
    xTaskCreate(&DisplayHandle::printResourceUsageTask, "ResourceUsageTask", 4096, this, 1, NULL);
}

void DisplayHandle::printSystemInfo() {
    clearAndUpdate();
    display.setCursor(0, 0);

    display.printf("CPU Freq: %d MHz\n", ESP.getCpuFreqMHz());
    display.printf("Free Heap: %d KB\n", ESP.getFreeHeap() / 1024);
    display.printf("Chip Model: \n%s\n", ESP.getChipModel());
    display.printf("Chip Rev: %d\n", ESP.getChipRevision());
    display.printf("Flash Size: %d MB\n", ESP.getFlashChipSize() / (1024 * 1024));
    display.printf("Flash Speed: %d MHz\n", ESP.getFlashChipSpeed() / 1000000);

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

void DisplayHandle::printBluetoothInfo() {
    clearAndUpdate();
    display.setCursor(0, 0);

    BluetoothHandle bluetoothHandle;
    while (true) {
        display.clearDisplay();
        display.setCursor(0, 0);
        display.println("Bluetooth Info");
        display.printf("This Device: \n%s\n", bluetoothHandle.getDeviceName());
        display.printf("Connected: %s\n", bluetoothHandle.isConnected() ? "Yes" : "No");

        display.display();
        vTaskDelay(1000 / portTICK_PERIOD_MS); // 1 second delay
    }
}

void DisplayHandle::printBluetoothInfoTask(void *pvParameters) {
    DisplayHandle *displayHandle = static_cast<DisplayHandle*>(pvParameters);
    displayHandle->printBluetoothInfo();
    vTaskDelete(NULL);
}

void DisplayHandle::startPrintBluetoothInfoTask() {
    xTaskCreate(&DisplayHandle::printBluetoothInfoTask, "BluetoothInfoTask", 4096, this, 1, NULL);
}

void DisplayHandle::printRadioInfo() {
    clearAndUpdate();
    display.setCursor(0, 0);

    RadioHandle radioHandle;
    while (true) {
        display.println("Radio Info");
        display.printf("Station: %s\n", radioHandle.getCurrentStation());
        display.printf("Frequency: %.1f FM\n", radioHandle.getFrequency());
        display.printf("Signal Level: %d\n", radioHandle.getSignalLevel());

        display.display();
        vTaskDelay(1000 / portTICK_PERIOD_MS); // 1 second delay
    }
}

void DisplayHandle::printRadioInfoTask(void *pvParameters) {
    DisplayHandle *displayHandle = static_cast<DisplayHandle*>(pvParameters);
    displayHandle->printRadioInfo();
    vTaskDelete(NULL);
}

void DisplayHandle::startPrintRadioInfoTask() {
    xTaskCreate(&DisplayHandle::printRadioInfoTask, "RadioInfoTask", 4096, this, 1, NULL);
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

void DisplayHandle::printAudioInfo() {
    // TODO
}

// status getter for display
bool DisplayHandle::isDisplayInitialized() const {
    return displayInitialized;
}

void DisplayHandle::clearAndUpdate() {
    display.clearDisplay();
    display.setCursor(0, 0);
}