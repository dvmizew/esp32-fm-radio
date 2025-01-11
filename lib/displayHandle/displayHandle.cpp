#include "displayHandle.h"

DisplayHandle::DisplayHandle()
    : I2C_display(1), // I2C bus 1
      display(DISPLAY_WIDTH, DISPLAY_HEIGHT, &I2C_display), // 128x64 display
      displayInitialized(false) {} // we init the display in the main program

void DisplayHandle::initDisplay() {
    I2C_display.begin(DISPLAY_SDA_PIN, DISPLAY_SCL_PIN, I2C_FREQUENCY); // start I2C bus with SDA and SCL pins
    if (!display.begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS)) {
        Serial.println(F("Error initializing display!"));
        while(true);
    }

    display.clearDisplay(); // clear display buffer
    display.setTextSize(TEXT_SIZE_SMALL);
    display.setTextColor(SSD1306_WHITE);
    displayInitialized = true;
}

void DisplayHandle::displayCustomMessage(const char *message) {
    clearAndUpdate();
    display.setTextSize(strlen(message) > 16 ? 1 : 2);
    display.println(message);
    display.display();
}

void DisplayHandle::displayCurrentDateTime() {
    while (true) {
        clearAndUpdate();
        
        configTime(0, 0, "pool.ntp.org", "time.nist.gov"); // get time from NTP server

        struct tm timeinfo;
        if (!getLocalTime(&timeinfo)) {
            display.println(F("Failed to obtain time\nPlease connect to \nWi-Fi"));
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

void DisplayHandle::displayDateTimeTask(void *pvParameters) {
    DisplayHandle *displayHandle = static_cast<DisplayHandle*>(pvParameters);
    displayHandle->displayCurrentDateTime();
    vTaskDelete(NULL);
}

void DisplayHandle::displayPrintDateTimeTask() {
    xTaskCreate(&DisplayHandle::displayDateTimeTask, "DateTimeTask", 2048, this, 1, NULL);
}

void DisplayHandle::displayResourceUsage() {
    clearAndUpdate();

    while (true) {
        // get heap info
        multi_heap_info_t heapInfo;
        heap_caps_get_info(&heapInfo, MALLOC_CAP_DEFAULT);

        size_t totalHeap = ESP.getHeapSize();
        size_t freeHeap = heapInfo.total_free_bytes;
        size_t usedHeap = totalHeap - freeHeap;
        size_t freeSPIFFFS = SPIFFS.totalBytes() - SPIFFS.usedBytes();
        size_t freePSRAM = ESP.getFreePsram();

        display.clearDisplay();
        display.setCursor(0, 0);
        display.println(F("RESOURCE USAGE\n"));
        display.printf("Total heap: %d KB\n", totalHeap / 1024);
        display.printf("Free Heap: %d KB\n", freeHeap / 1024);
        display.printf("Used Heap: %d KB\n", usedHeap / 1024);
        if (freePSRAM > 0) // check if PSRAM is available
            display.printf("Free PSRAM: %d KB\n", freePSRAM / 1024);
        if (freeSPIFFFS > 0)
            display.printf("Free SPIFFS: %d KB\n", freeSPIFFFS / 1024);
        display.printf("Chip Temp: %0.2f C\n", (double)temperatureRead());
        display.display();

        vTaskDelay(1000 / portTICK_PERIOD_MS); // 1 second delay
    }
}

void DisplayHandle::displayResourceUsageTask(void *pvParameters) {
    DisplayHandle *displayHandle = static_cast<DisplayHandle*>(pvParameters);
    displayHandle->displayResourceUsage();
    vTaskDelete(NULL);
}

void DisplayHandle::displayPrintResourceUsageTask() {
    xTaskCreate(&DisplayHandle::displayResourceUsageTask, "ResourceUsageTask", 2048, this, 1, NULL);
}

void DisplayHandle::displaySystemInfo() {
    clearAndUpdate();

    display.printf("CPU Freq: %d MHz\n", ESP.getCpuFreqMHz());
    display.printf("Total Heap: %d KB\n", ESP.getHeapSize() / 1024);
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

void DisplayHandle::displaySPIFFSInfo() {
    clearAndUpdate();

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
void DisplayHandle::displayWiFiNetworks() {
    clearAndUpdate();

    int availableNetworkCount = WiFi.scanNetworks();
    display.println("Wi-Fi Networks");
    display.printf("Total: %d\n", availableNetworkCount);
    for (int i = 0; i < availableNetworkCount; ++i) {
        display.printf("%d: %s\n", i + 1, WiFi.SSID(i).c_str());
    }
    display.display();
}

void DisplayHandle::displayWiFiConnectionStatus() {
    clearAndUpdate();

    if (isWiFiConnected()) {
        display.println("Wi-Fi Connected\n");

        display.printf("SSID: %s\n", WiFi.SSID().c_str());

        display.printf("IP: %s\n", WiFi.localIP().toString().c_str());

        display.printf("RSSI: %d dBm\n", WiFi.RSSI());
    } else {
        display.println("Wi-Fi Not Connected");
    }

    display.display();
}

void DisplayHandle::displayBluetoothInfo() {
    clearAndUpdate();

    while (true) {
        display.clearDisplay();
        display.setCursor(0, 0);
        display.printf("%s\n\n", getBluetoothDeviceName());
        display.printf("Connected: %s\n", bluetoothIsConnected() ? "Yes" : "No");
        display.printf("Title: %s\n", currentTitle.c_str());
        display.printf("Artist: %s\n", currentArtist.c_str());
        display.printf("Album: %s\n", currentAlbum.c_str());

        display.display();
        vTaskDelay(1000 / portTICK_PERIOD_MS); // 1 second delay
    }
}

void DisplayHandle::displayBluetoothInfoTask(void *pvParameters) {
    DisplayHandle *displayHandle = static_cast<DisplayHandle*>(pvParameters);
    displayHandle->displayBluetoothInfo();
    vTaskDelete(NULL);
}

void DisplayHandle::displayPrintBluetoothInfoTask() {
    xTaskCreate(&DisplayHandle::displayBluetoothInfoTask, "BluetoothInfoTask", 2048, this, 1, NULL);
}

void DisplayHandle::displayRadioInfo() {
    clearAndUpdate();

    while (true) {
        display.clearDisplay();
        display.setCursor(0, 0);
        display.println("Radio Info");
        display.printf("Station: %s\n", getCurrentStation());
        display.printf("Frequency: %.1f FM\n", getFrequency());
        display.printf("Signal Level: %d\n", getSignalLevel());

        display.display();
        vTaskDelay(1000 / portTICK_PERIOD_MS); // 1 second delay
    }
}

void DisplayHandle::displayRadioInfoTask(void *pvParameters) {
    DisplayHandle *displayHandle = static_cast<DisplayHandle*>(pvParameters);
    displayHandle->displayRadioInfo();
    vTaskDelete(NULL);
}

void DisplayHandle::startRadioInfoDisplayTask() {
    xTaskCreate(&DisplayHandle::displayRadioInfoTask, "RadioInfoTask", 2048, this, 1, NULL);
}

void DisplayHandle::displayWeatherInfo() {
    // TODO
}

void DisplayHandle::displayRDSInfo() {
    // TODO
}

void DisplayHandle::displayBluetoothConnectionStatus() {
    clearAndUpdate();

    if (bluetoothIsConnected()) {
        display.println("Bluetooth Connected");
        display.printf("Device: %s\n", getBluetoothDeviceName());

        // display.printf("RSSI: %d dBm\n", bluetoothHandle.getSignalStrength());
    } else {
        display.println("Bluetooth Not Connected");
    }

    display.display();
}

void DisplayHandle::displayAudioInfo() {
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