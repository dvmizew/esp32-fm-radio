#include <Arduino.h>
#include <unity.h>
#include "displayHandle.h"
#include "hardware.h"
#include "radioHandle.h"
#include "wifiHandle.h"
#include "bluetooth.h"
#include "webserverHandle.h"

extern bool displayInitialized;
extern WiFiCredentials savedNetworks[MAX_NETWORKS];
extern uint8_t savedNetworksCount;
extern uint8_t nextID;
extern bool bluetoothSpeakerInitialized;

const char* ssid = "hehe";
const char* password = "ciscosecpa55";

// Test cases for displayHandle
void test_initDisplay() {
    initDisplay();
    TEST_ASSERT_TRUE(displayInitialized);
}

// Test cases for hardware
void test_setupButtons() {
    setupButtons();
    TEST_ASSERT_EQUAL(INPUT_PULLUP, getPinMode(PLAY_BUTTON));
    TEST_ASSERT_EQUAL(INPUT_PULLUP, getPinMode(NEXT_BUTTON));
    TEST_ASSERT_EQUAL(INPUT_PULLUP, getPinMode(PREV_BUTTON));
}

void test_handleButtonPress() {
    bool buttonHeld = false;
    unsigned long buttonPressTime = 0;
    bool shortPressCalled = false;
    bool longPressCalled = false;

    // Mock short press action
    std::function<void()> shortPressAction = [&shortPressCalled]() {
        shortPressCalled = true;
        Serial.println("Short press action called");
    };

    // Mock long press action
    std::function<void()> longPressAction = [&longPressCalled]() {
        longPressCalled = true;
        Serial.println("Long press action called");
    };

    // Simulate short press
    Serial.println("Simulating short press");
    digitalWrite(PLAY_BUTTON, LOW);
    delay(100);
    handleButtonPress(PLAY_BUTTON, buttonHeld, buttonPressTime, shortPressAction, longPressAction);
    digitalWrite(PLAY_BUTTON, HIGH);
    handleButtonPress(PLAY_BUTTON, buttonHeld, buttonPressTime, shortPressAction, longPressAction);
    Serial.printf("shortPressCalled: %d, longPressCalled: %d\n", shortPressCalled, longPressCalled);
    TEST_ASSERT_TRUE(shortPressCalled);
    TEST_ASSERT_FALSE(longPressCalled);

    // Reset for long press test
    shortPressCalled = false;
    longPressCalled = false;
    buttonHeld = false;
    buttonPressTime = 0;

    // Simulate long press
    Serial.println("Simulating long press");
    digitalWrite(PLAY_BUTTON, LOW);
    delay(BUTTON_HOLD_TIME + 100);
    handleButtonPress(PLAY_BUTTON, buttonHeld, buttonPressTime, shortPressAction, longPressAction);
    digitalWrite(PLAY_BUTTON, HIGH);
    handleButtonPress(PLAY_BUTTON, buttonHeld, buttonPressTime, shortPressAction, longPressAction);
    Serial.printf("shortPressCalled: %d, longPressCalled: %d\n", shortPressCalled, longPressCalled);
    TEST_ASSERT_FALSE(shortPressCalled);
    TEST_ASSERT_TRUE(longPressCalled);
}

// Test cases for radioHandle
void test_initRadio() {
    initRadio();
    TEST_ASSERT_EQUAL_FLOAT(107.5, getFrequency());
}

void test_increaseRadioFrequency() {
    float initialFrequency = getFrequency();
    increaseRadioFrequency();
    TEST_ASSERT_EQUAL_FLOAT(initialFrequency + 0.1, getFrequency());
}

void test_decreaseRadioFrequency() {
    float initialFrequency = getFrequency();
    decreaseRadioFrequency();
    TEST_ASSERT_EQUAL_FLOAT(initialFrequency - 0.1, getFrequency());
}

void test_setRadioFrequency() {
    setRadioFrequency(100.0);
    TEST_ASSERT_EQUAL_FLOAT(100.0, getFrequency());
}

void test_addRemoveWiFiCredentials() {
    // Add WiFi credentials
    addWiFiCredentials(savedNetworks, &savedNetworksCount, &nextID, ssid, password);
    TEST_ASSERT_EQUAL_STRING(ssid, savedNetworks[0].ssid);
    TEST_ASSERT_EQUAL_UINT8(1, savedNetworksCount);

    // Remove WiFi credentials
    Serial.printf("Before removal: savedNetworksCount: %d\n", savedNetworksCount);
    removeWiFiCredentials(savedNetworks, &savedNetworksCount, ssid);
    Serial.printf("After removal: savedNetworksCount: %d\n", savedNetworksCount);
    TEST_ASSERT_EQUAL_UINT8(0, savedNetworksCount);
}

void test_ConnectDisconnectWiFiNetwork() {
    // Connect to WiFi network
    connectToWiFiNetwork(ssid, password);
    TEST_ASSERT_TRUE(isWiFiConnected());

    // Disconnect from WiFi network
    WiFi.disconnect();
    delay(1000); // Allow some time for disconnection
    TEST_ASSERT_FALSE(isWiFiConnected());
}

// Test cases for bluetooth
void test_initializeBluetoothSpeaker() {
    initializeBluetoothSpeaker();
    TEST_ASSERT_TRUE(bluetoothSpeakerInitialized);
}

// Test cases for webserverHandle
void test_initSPIFFS() {
    initSPIFFS();
    TEST_ASSERT_TRUE(SPIFFS.begin());
}

// Test case for memory corruption
void test_memoryCorruption() {
    // Check initial free heap
    size_t initialFreeHeap = ESP.getFreeHeap();
    Serial.printf("Initial free heap: %d\n", initialFreeHeap);

    // Allocate and deallocate memory
    const size_t testSize = 1024;
    uint8_t* testMemory = (uint8_t*)malloc(testSize);
    TEST_ASSERT_NOT_NULL(testMemory);
    memset(testMemory, 0xAA, testSize);
    free(testMemory);

    // Check free heap after allocation and deallocation
    size_t finalFreeHeap = ESP.getFreeHeap();
    Serial.printf("Final free heap: %d\n", finalFreeHeap);

    // Check for heap integrity
    TEST_ASSERT_EQUAL(initialFreeHeap, finalFreeHeap);

    // Monitor stack usage
    size_t stackHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
    Serial.printf("Stack high water mark: %d\n", stackHighWaterMark);
    TEST_ASSERT_GREATER_THAN(100, stackHighWaterMark); // Ensure at least 100 bytes of stack space remain
}

// Combined test case for web server, Bluetooth, Wi-Fi, and radio working together
void test_combinedFunctionality() {
    // Cleanup before test
    WiFi.disconnect(true);
    delay(1000); // Allow some time for disconnection
    if (bluetoothSpeakerInitialized) {
        disableBluetoothSpeaker();
        bluetoothSpeakerInitialized = false;
    }

    // Check initial free heap
    size_t initialFreeHeap = ESP.getFreeHeap();
    Serial.printf("Initial free heap: %d\n", initialFreeHeap);

    // Initialize and start web server
    initSPIFFS();
    setupWebServer();

    // Initialize and start Bluetooth
    initializeBluetoothSpeaker();
    TEST_ASSERT_TRUE(bluetoothSpeakerInitialized);

    // Connect to WiFi network
    connectToWiFiNetwork(ssid, password);
    TEST_ASSERT_TRUE(isWiFiConnected());

    // Initialize and start radio
    initRadio();
    TEST_ASSERT_EQUAL_FLOAT(107.5, getFrequency());

    // Check free heap after initialization
    size_t finalFreeHeap = ESP.getFreeHeap();
    Serial.printf("Final free heap: %d\n", finalFreeHeap);

    // Check for heap integrity
    TEST_ASSERT_GREATER_THAN(initialFreeHeap - 5000, finalFreeHeap); // Ensure no significant memory leak

    // Monitor stack usage
    size_t stackHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
    Serial.printf("Stack high water mark: %d\n", stackHighWaterMark);
    TEST_ASSERT_GREATER_THAN(100, stackHighWaterMark); // Ensure at least 100 bytes of stack space remain

    // Clean up
    WiFi.disconnect();
    TEST_ASSERT_FALSE(isWiFiConnected());
}

void setup() {
    UNITY_BEGIN();
    RUN_TEST(test_initDisplay);
    RUN_TEST(test_setupButtons);
    RUN_TEST(test_handleButtonPress);
    RUN_TEST(test_initRadio);
    RUN_TEST(test_increaseRadioFrequency);
    RUN_TEST(test_decreaseRadioFrequency);
    RUN_TEST(test_setRadioFrequency);
    RUN_TEST(test_ConnectDisconnectWiFiNetwork);
    RUN_TEST(test_addRemoveWiFiCredentials);
    RUN_TEST(test_initializeBluetoothSpeaker);
    RUN_TEST(test_initSPIFFS);
    RUN_TEST(test_memoryCorruption);
    RUN_TEST(test_combinedFunctionality);
    UNITY_END();
}

void loop() { }

int main(int argc, char** argv) {
    setup();
    while (true) {
        loop();
    }
    return 0;
}