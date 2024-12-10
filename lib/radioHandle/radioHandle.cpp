// #include <Wire.h>
// #include <TEA5767.h>
#include "radioHandle.h"
#include "hardware.h"

RadioHandle::RadioHandle() {
    // default values (lowest frequency that TEA5767 supports)
    snprintf(currentStation, sizeof(currentStation), "87.5 FM");
    frequency = 87.5;
}

void RadioHandle::setupRadio() {
    Wire.begin(TEA5767_SDA, TEA5767_SCL); // start I2C bus with radio pins
    Wire.setClock(100000); // radio clock
}

void RadioHandle::initRadio() {
    setupRadio();
    radio.init();
    radio.setFrequency(frequency);  // Set initial frequency to 87.5 FM
    Serial.println(F("Radio initialized!"));
    Serial.println(F("Current station:"));
    Serial.println(frequency);
}

void RadioHandle::tuneRadio(float freq) {
    if (freq >= 87.5 && freq <= 108.0) {
        radio.setFrequency(freq);  // Set the radio to the desired frequency
        snprintf(currentStation, sizeof(currentStation), "%.1f FM", freq);
    } else {
        Serial.println(F("Invalid frequency!"));
    }
}

void RadioHandle::searchRadioStations() {
    for (float i = 87.5; i <= 108.0; i += 0.1) {
        radio.setFrequency(i);
        Serial.print(F("Searching station at "));
        Serial.print(i);
        tuneRadio(i); // iterate through all frequencies
        delay(1000);
    }
}

void RadioHandle::handleRadioControl() {
    // TODO
}

const char* RadioHandle::getCurrentStation() const {
    return currentStation;
}

float RadioHandle::getFrequency() const {
    return frequency;
}