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

// test cases for displayHandle
void test_initDisplay() {
    initDisplay();
    TEST_ASSERT_TRUE(displayInitialized);
}

// test cases for hardware
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

    // mock short press action
    std::function<void()> shortPressAction = [&shortPressCalled]() {
        shortPressCalled = true;
        Serial.println("Short press action called");
    };

    // mock long press action
    std::function<void()> longPressAction = [&longPressCalled]() {
        longPressCalled = true;
        Serial.println("Long press action called");
    };

    // simulate short press
    Serial.println("Simulating short press");
    digitalWrite(PLAY_BUTTON, LOW);
    delay(100);
    handleButtonPress(PLAY_BUTTON, buttonHeld, buttonPressTime, shortPressAction, longPressAction);
    digitalWrite(PLAY_BUTTON, HIGH);
    handleButtonPress(PLAY_BUTTON, buttonHeld, buttonPressTime, shortPressAction, longPressAction);
    TEST_ASSERT_TRUE(shortPressCalled);
    TEST_ASSERT_FALSE(longPressCalled);

    // reset for long press test
    shortPressCalled = false;
    longPressCalled = false;
    buttonHeld = false;
    buttonPressTime = 0;

    // simulate long press
    Serial.println("Simulating long press");
    digitalWrite(PLAY_BUTTON, LOW);
    delay(BUTTON_HOLD_TIME + 100);
    handleButtonPress(PLAY_BUTTON, buttonHeld, buttonPressTime, shortPressAction, longPressAction);
    digitalWrite(PLAY_BUTTON, HIGH);
    handleButtonPress(PLAY_BUTTON, buttonHeld, buttonPressTime, shortPressAction, longPressAction);
    TEST_ASSERT_FALSE(shortPressCalled);
    TEST_ASSERT_TRUE(longPressCalled);
}

// test cases for radioHandle
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

// test cases for wifiHandle
void test_connectToWiFiNetwork() {
    connectToWiFiNetwork("hehe", "ciscosecpa55");
    TEST_ASSERT_TRUE(isWiFiConnected());
}

void test_addWiFiCredentials() {
    addWiFiCredentials(savedNetworks, &savedNetworksCount, &nextID, "hehe", "ciscosecpa55");
    TEST_ASSERT_EQUAL_STRING("hehe", savedNetworks[0].ssid);
}

void test_removeWiFiCredentials() {
    addWiFiCredentials(savedNetworks, &savedNetworksCount, &nextID, "hehe", "ciscosecpa55");
    removeWiFiCredentials(savedNetworks, &savedNetworksCount, "hehe");
    TEST_ASSERT_EQUAL_UINT8(0, savedNetworksCount);
}

void test_clearAllWiFiCredentials() {
    addWiFiCredentials(savedNetworks, &savedNetworksCount, &nextID, "hehe", "ciscosecpa55");
    clearAllWiFiCredentials(savedNetworks, &savedNetworksCount, &nextID);
    TEST_ASSERT_EQUAL_UINT8(0, savedNetworksCount);
}

// test cases for bluetooth
void test_initializeBluetoothSpeaker() {
    initializeBluetoothSpeaker();
    TEST_ASSERT_TRUE(bluetoothSpeakerInitialized);
}

// test cases for webserverHandle
void test_initSPIFFS() {
    initSPIFFS();
    TEST_ASSERT_TRUE(SPIFFS.begin());
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
    RUN_TEST(test_connectToWiFiNetwork);
    RUN_TEST(test_addWiFiCredentials);
    RUN_TEST(test_removeWiFiCredentials);
    RUN_TEST(test_clearAllWiFiCredentials);
    RUN_TEST(test_initializeBluetoothSpeaker);
    RUN_TEST(test_initSPIFFS);
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