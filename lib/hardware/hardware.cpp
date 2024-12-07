#include <WiFi.h>
#include "hardware.h"
#include "radioHandle.h"
#include "bluetooth.h"

void setupHardware() {
    pinMode(BUTTON_PLAY, INPUT_PULLUP);
    pinMode(JOYSTICK_SW, INPUT_PULLUP);
    Wire.begin(DISPLAY_SDA_PIN, DISPLAY_SCL_PIN);
}

void showAvailableWiFiNetworks() {
    Serial.println("Scanning for available Wi-Fi networks...");

    WiFi.mode(WIFI_STA); // set ESP32 to station mode
    WiFi.disconnect(); // disconnect from any other network
    delay(100);

    int networkCount = WiFi.scanNetworks(); // scan for available networks
    Serial.println("Scan done");

    if (networkCount == 0)
        Serial.println("No networks found");

    else {
        Serial.printf("%d networks found\n", networkCount);
        Serial.println("Nr | SSID                             | RSSI | CH | Encryption");
        for (int i = 0; i < networkCount; ++i) {
            Serial.printf("%2d | %-32.32s | %4d | %2d | ", i + 1, WiFi.SSID(i).c_str(), WiFi.RSSI(i), WiFi.channel(i));
            switch (WiFi.encryptionType(i)) {
                case WIFI_AUTH_OPEN:
                    Serial.print("open");
                    break;
                case WIFI_AUTH_WEP:
                    Serial.print("WEP");
                    break;
                case WIFI_AUTH_WPA_PSK:
                    Serial.print("WPA");
                    break;
                case WIFI_AUTH_WPA2_PSK:
                    Serial.print("WPA2");
                    break;
                case WIFI_AUTH_WPA_WPA2_PSK:
                    Serial.print("WPA+WPA2");
                    break;
                case WIFI_AUTH_WPA2_ENTERPRISE:
                    Serial.print("WPA2-EAP");
                    break;
                case WIFI_AUTH_WPA3_PSK:
                    Serial.print("WPA3");
                    break;
                case WIFI_AUTH_WPA2_WPA3_PSK:
                    Serial.print("WPA2+WPA3");
                    break;
                case WIFI_AUTH_WAPI_PSK:
                    Serial.print("WAPI");
                    break;
                default:
                    Serial.print("unknown");
                    break;
            }
            Serial.println();
            delay(10);  // delay to allow serial monitor to process output
        }
    }

    WiFi.scanDelete(); // free memory used by scan results
}

void connectToWifi(const char *ssid, const char *password) {
    WiFi.begin(ssid, password);
    Serial.println("Connecting to WiFi...");

    unsigned long startAttemptTime = millis();
    int retryCount = 0;
    const int maxRetries = 5;
    const int retryDelay = 2000;

    while (WiFi.status() != WL_CONNECTED && retryCount < maxRetries) {
        // check if connection is successful
        if (WiFi.status() == WL_CONNECTED) {
            Serial.println("Connected!");
            Serial.print("IP Address: ");
            Serial.println(WiFi.localIP());
            Serial.println("MAC Address: ");
            Serial.println(WiFi.macAddress());
            return; // success
        }
        // retry connection
        retryCount++;
        Serial.printf("Connection attempt %d of %d...\n", retryCount, maxRetries);
        delay(retryDelay);
        if (retryCount >= maxRetries) {
            Serial.println("Connection failed, max retries reached.");
            return;
        }
    }
}

unsigned long lastDebounceTime = 0;

void handleJoystickInput() {
    unsigned long currentTime = millis();

    if (currentTime - lastDebounceTime > debounceDelay) {
        lastDebounceTime = currentTime;
        
        if (digitalRead(JOYSTICK_SW) == LOW) {
            frequency += 0.1;
            if (frequency > 108.0) 
                frequency = 87.5;
            tuneRadio(frequency);
        }

        if (digitalRead(BUTTON_PLAY) == LOW) {
            initializeBluetoothSpeaker();
        }
    }
}